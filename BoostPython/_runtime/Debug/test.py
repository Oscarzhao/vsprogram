__author__ = "zhaosl"
#encoding=utf8
from XtTraderPyApi import *

class Callback(XtTraderApiCallback):
    def __init__(self, address):
        self.m_strAddress = address
        self.m_nRequestId = 0
        self.m_client = None

    def init(self):
        if self.m_strAddress == '':
            return -1, u'server addr is empty'
        self.m_client = XtTraderApi.createXtTraderApi(self.m_strAddress)
        if None == self.m_client:
            return -1, u'failed to create traderapi client'
        if isinstance(self, XtTraderApiCallback):
            self.m_client.setCallback(self)
            # self.m_client.setCallback(super(XtTraderApiCallback, self))
        self.m_client.init()
        return 0, u''

    def join(self):
        self.m_client.join()

if __name__ == '__main__':
    # cs = CString()
    # print "Before, default: cs.m_nInt = ", cs.m_nInt
    # cs.m_nInt = 5
    # print "After,  default: cs.m_nInt = ", cs.m_nInt
    #
    # print 'Test C-String'
    # print 'cs.m_strArray = %s' % cs.m_strArray
    # vec = cs.m_vec
    # print "type of cs.m_vec: ", type(vec)
    # vec.append(4)
    # vec.append(5)
    # print "content of vec: %d, %d" %(vec[0], vec[1])
    #
    # wd = World()
    # wd.readCString(cs)

    cb = Callback('192.168.1.107:8000')
    cb.init()
    cb.join()