//
// io_service_pool.hpp
//

#ifndef UTILS_THREAD_IO_SERVICE_POOL_HPP
#define UTILS_THREAD_IO_SERVICE_POOL_HPP

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <vector>
#include <deque>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace utils {
    namespace thread {

        typedef boost::shared_ptr<boost::asio::io_service> io_service_ptr;
        typedef boost::shared_ptr<boost::asio::io_service::work> work_ptr;
        typedef unsigned char byte;

        struct ext_io_service
        {
            io_service_ptr ios;
            work_ptr work;
            boost::shared_ptr<boost::thread> trd;
            int use_count;
            int idle_begin_time;      // start time of idle

            ext_io_service() : use_count(0), idle_begin_time((int)time(NULL)) {};
        };

        typedef boost::shared_ptr<ext_io_service> ext_io_service_ptr;

        // A pool of io_service objects.
        class io_service_pool
            : private boost::noncopyable
        {
        public:
            // Construct the io_service pool.
            static io_service_pool& instance()
            {
                static io_service_pool s_ios_pool;
                return s_ios_pool;
            }

            explicit io_service_pool(std::size_t pool_size = 0, std::size_t max_pool_size = 600, int timeout_interval = 0);
            void init(std::size_t pool_size = 0, std::size_t max_pool_size = 600, int timeout_interval = 0);

            // Run all io_service objects in the pool.
            void run();

            // Stop all io_service objects in the pool.
            void stop();

            // Get an io_service pointer
            io_service_ptr get_io_service();
            io_service_ptr get_io_service_exclusive(); // throw std::exception

            // to recover the working io with the tag is the address of the io_service
            void complete(long tag);

        private:
            ext_io_service_ptr create();

            const ext_io_service_ptr& get_work_ios();
            void recycle(const ext_io_service_ptr& io_service);
            void adjust();

        private:
            // size of pool
            std::size_t pool_size_;
            std::size_t max_pool_size_;
            // The pool of working io_services
            std::map<long, ext_io_service_ptr> work_io_services_;
            // The pool of exclusive io_services
            std::map<long, ext_io_service_ptr> exclusive_io_services_;
            // The pool of idle io_services
            std::deque<ext_io_service_ptr> idle_io_services_;
            // timeout interval for idle io_service
            int timeout_interval_;
            boost::mutex mutex_;
        };

    } // namespace thread
} // namespace utils

#endif // UTILS_THREAD_IO_SERVICE_POOL_HPP
