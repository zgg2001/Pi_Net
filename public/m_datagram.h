/*************************************************************************
	> File Name: m_datagram.h
	> Author: ZHJ
	> Remarks: 数据报文协议
	> Created Time: Fri 20 May 2022 04:56:08 PM CST
 ************************************************************************/

#ifndef _M_DATAGRAM_H_
#define _M_DATAGRAM_H_

/*
* 命令(报文)类型
*/
enum cmd
{
    CMD_C2S_HEART,      //心跳 client to server
    CMD_S2C_HEART,      //心跳 server to client
    CMD_C2S_DATA,       //数据 树莓派->server
    CMD_C2S_DATA_RESULT,//数据传输结果 树莓派->server
};

/*
* 报头
*    cmd: 报文命令
* length: 报文长度
*/
struct header
{
    short cmd;
    short length;
};

/*
* 心跳报文 C2S
*/
struct c2s_heart : public header
{
    c2s_heart()
    {
        this->cmd = CMD_C2S_HEART;
        this->length = sizeof(c2s_heart);
    }
};

/*
* 心跳报文 S2C
*/
struct s2c_heart : public header
{
    s2c_heart()
    {
        this->cmd = CMD_S2C_HEART;
        this->length = sizeof(s2c_heart);
    }
};

/*
* 数据报文 树莓派->server
*/
struct c2s_data : public header
{
    c2s_data()
    {
        this->cmd = CMD_C2S_DATA;
        this->length = sizeof(c2s_data);
    }
    int id;//传感器id
    int Temp;//室内温度
    int Rh;//室内湿度
};

/*
* 数据传输结果 树莓派->server
*/
struct c2s_data_result : public header
{
    c2s_data_result()
    {
        this->cmd = CMD_C2S_DATA_RESULT;
        this->length = sizeof(c2s_data_result);
    }
    int result;
};

#endif
