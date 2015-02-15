//
// io_service_pool.cpp
//
#include <stdexcept>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include "io_service_pool.hpp"

namespace utils {
    namespace thread {

        long get_tag(const ext_io_service_ptr& io_service)
        {
            return reinterpret_cast<long>(io_service->ios.get());
        }

        struct find_by_tag : public std::binary_function<ext_io_service_ptr, int, bool>
        {
            bool operator() (const ext_io_service_ptr& ext_ios, int tag) const
            {
                return tag == get_tag(ext_ios);
            }
        };

        bool less_use(const std::pair<int, ext_io_service_ptr>& left, const std::pair<int, ext_io_service_ptr>& right)
        {
            return left.second->use_count < right.second->use_count;
        }

        // ---------------------------------------------------------------------

        io_service_pool::io_service_pool(std::size_t pool_size, std::size_t max_pool_size, int timeout_interval)
        {
            init(pool_size, max_pool_size, timeout_interval);
        }

        void io_service_pool::init(std::size_t pool_size, std::size_t max_pool_size, int timeout_interval)
        {
            max_pool_size_ = max_pool_size > 0 ? max_pool_size : 600;
            pool_size_ = pool_size > max_pool_size_ ? max_pool_size_ : pool_size;
            timeout_interval_ = timeout_interval;

            for (std::size_t i = 0; i < pool_size_; ++i)
            {
                try 
                {
                    ext_io_service_ptr io_service = create();
                    idle_io_services_.push_back(io_service);
                }
                catch (const std::exception& e)
                {
                    printf("create io error in init, %s\n", e.what());
                }
            }
        }

        ext_io_service_ptr io_service_pool::create()
        {
            // Give the created io_services work to do so that their run() functions will not
            // exit until they are explicitly stopped.
            ext_io_service_ptr ext_ios = ext_io_service_ptr(new ext_io_service());
            ext_ios->ios.reset(new boost::asio::io_service);
            ext_ios->work.reset(new boost::asio::io_service::work(*ext_ios->ios));
            ext_ios->trd.reset(new boost::thread(boost::bind(&boost::asio::io_service::run, ext_ios->ios)));
            ext_ios->idle_begin_time = (int)time(NULL);
            return ext_ios;
        }

        void io_service_pool::run()
        {
            NULL;
        }

        void io_service_pool::stop()
        {
            // Explicitly stop all io_services.
            boost::mutex::scoped_lock lock(mutex_);

            std::map<long, ext_io_service_ptr>::iterator iter = work_io_services_.begin();
            std::map<long, ext_io_service_ptr>::iterator iter_end = work_io_services_.end();
            for (; iter != iter_end; ++iter)
            {
                iter->second->ios->stop();
            }
            work_io_services_.clear();

            std::map<long, ext_io_service_ptr>::iterator iter2 = exclusive_io_services_.begin();
            std::map<long, ext_io_service_ptr>::iterator iter2_end = exclusive_io_services_.end();
            for (; iter2 != iter2_end; ++iter2)
            {
                iter2->second->ios->stop();
            }
            exclusive_io_services_.clear();

            while (idle_io_services_.size() > pool_size_)
            {
                idle_io_services_.front()->ios->stop();
                idle_io_services_.pop_front();
            }
        }

        const ext_io_service_ptr& io_service_pool::get_work_ios()
        {
            if (work_io_services_.empty())
                throw std::runtime_error("empty work io service");

            printf("reuse work io, total: %d\n", work_io_services_.size());
            std::map<long, ext_io_service_ptr>::iterator iter = std::min_element(work_io_services_.begin(), work_io_services_.end(), less_use);
            return iter->second;
        }

        io_service_ptr io_service_pool::get_io_service()
        {
            boost::mutex::scoped_lock lock(mutex_);

            ext_io_service_ptr io_service;
            if (idle_io_services_.empty())
            {
                if (work_io_services_.size() < max_pool_size_)
                {
                    try 
                    {
                        io_service = create();
                        work_io_services_[get_tag(io_service)] = io_service;
                        printf("create io, total: %d\n", work_io_services_.size());
                    }
                    catch (const std::exception& e)
                    {
                        printf("create io failed, %s\n", e.what());
                        io_service = get_work_ios();
                    }
                }
                else
                {
                    io_service = get_work_ios();
                }
            }
            else
            {
                // get newly in
                io_service = idle_io_services_.back();
                idle_io_services_.pop_back();
                work_io_services_[get_tag(io_service)] = io_service;
            }

            ++(io_service->use_count);
            return io_service->ios;
        }

        io_service_ptr io_service_pool::get_io_service_exclusive()
        {
            boost::mutex::scoped_lock lock(mutex_);

            ext_io_service_ptr io_service;
            if (idle_io_services_.empty())
            {
                try 
                {
                    io_service = create();
                    exclusive_io_services_[get_tag(io_service)] = io_service;
                    printf("create io, exclusive: %d\n", exclusive_io_services_.size());
                }
                catch (const std::exception& e)
                {
                    printf("create exclusive io failed, %s\n", e.what());
                    throw e;
                }
            }
            else
            {
                io_service = idle_io_services_.back();
                idle_io_services_.pop_back();
                exclusive_io_services_[get_tag(io_service)] = io_service;
            }
            return io_service->ios;
        }

        void io_service_pool::complete(long tag)
        {
            boost::mutex::scoped_lock lock(mutex_);

            std::map<long, ext_io_service_ptr>::iterator iter_f = work_io_services_.find(tag);
            if (iter_f != work_io_services_.end())
            {
                --(iter_f->second->use_count);
                if (0 == iter_f->second->use_count)
                {
                    recycle(iter_f->second);
                    work_io_services_.erase(iter_f);
                }
            }
            else
            {
                std::map<long, ext_io_service_ptr>::iterator iter2_f = exclusive_io_services_.find(tag);
                if (iter2_f != exclusive_io_services_.end())
                {
                    recycle(iter2_f->second);
                    exclusive_io_services_.erase(iter2_f);
                }
            }
        }

        void io_service_pool::recycle(const ext_io_service_ptr& io_service)
        {
            io_service->idle_begin_time = (int)time(NULL);
            idle_io_services_.push_back(io_service);
            adjust();
            printf("io stat, work: %d, idle: %d, exclusive: %d\n", work_io_services_.size(), idle_io_services_.size(), exclusive_io_services_.size());
        }

        void io_service_pool::adjust()
        {
            int now = (int)time(NULL);
            while (idle_io_services_.size() > pool_size_ 
                && now - idle_io_services_.front()->idle_begin_time >= timeout_interval_)
            {
                printf("destroy io\n");
                idle_io_services_.pop_front();
            }
        }

    } // namespace thread
} // namespace utils
