/**
* 客户端Login Cmd定义
* 目前用于proxy passport和client
*
* @author   zld
* @date     2011-12-27
*/


#ifndef __LOGIN_COMMAND__
#define __LOGIN_COMMAND__

enum ClientType
{
    TYPE_PC         =   0,  //客户端是pc
    TYPE_MOBILE             //客户端是手机
};

enum LoginCmd
{
    CMD_LOGIN       =   0,  //正常登录请求
    CMD_LOGOUT,             //退出请求
    CMD_RELOGIN,            //补登录请求，用于proxy和passport断开重连的时候
    CMD_LOGIN_OFFLINE,      //离线登录，用于单机版
};

enum InfoType
{
    INFO_TYPE_CLIENT = 0, //clientInfo, 包括用户mac地址等
    INFO_TYPE_CONFIG,     //软件配置信息
};

//passport对proxy主动下达的命令
enum LoginServerCmd
{
    CMD_KICKOUT     =   0,  //踢人
};

//不使用bool是为今后各种失败原因分别处理做扩展
enum LoginResult
{
    RESULT_SUCCESS  =   0,  //成功
    RESULT_FAILURE,         //失败
    RESULT_FAILURE_EXCEED,   //失败，超出限制
    RESULT_FAILURE_NOACTIVE, //失败，未激活
    RESULT_FAILURE_EXPIRED,  //失败，已过期
    RESULT_FAILURE_VERSION_EXPIRED, // 版本过低
    RESULT_FAILURE_SERVICE_NOTREADY,  //source server 或 passport server 没有准备好服务
    RESULT_FAILURE_VERSION_DISABLE, // 版本被禁用
    
};

enum NotifyType
{
    NOTIFYTYPE_DELUSER = 0, //删除子用户
    NOTIFYTYPE_ADDAUTH,     //增加了授权
    NOTIFYTYPE_DELAUTH,      //取消了授权
    NOTIFYTYPE_DISABLEUSER  //停用子用户
};

enum ACTIVESTATUS
{
    ACTIVESTATUS_NOTVALID = 0, //未激活
    ACTIVESTATUS_VALID
};



#endif //__LOGIN_COMMAND__