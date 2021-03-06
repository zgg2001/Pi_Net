/*************************************************************************
	> File Name: m_db.h
	> Author: ZHJ
	> Remarks: 连接MySql数据库
	> Created Time: Tue 24 May 2022 06:43:58 PM CST
 ************************************************************************/

#ifndef _M_DB_H_
#define _M_DB_H_

#include<vector>
#include<mutex>
#include<mysql/mysql.h>

class m_db
{
public:
    m_db();
    ~m_db();
    m_db(const m_db&) = delete;
    m_db& operator= (const m_db&) = delete;

    /* 
    * 连接MySql
    *
    *    host: host
    *    user: 登录用户名
    *  passwd: 登录密码
    * db_name: 数据库名
    *
    * 返回值:
    *   -2 : 失败 -- MYSQL对象为空(mysql_init error)
    *   -1 : 失败 -- connect失败
    *    0 : 成功
    */
    int connect(const char* host, const char* user, 
                const char* passwd, const char* db_name);

    /*
    * 执行指定sql语句
    *
    *        sql: 想要执行的sql语句
    *        res: 储存查询结果
    *   num_rows: 结果的行数
    * num_fields: 结果的字段数
    *
    * 返回 -1 执行失败
    * 返回  0 执行成功且有执行结果(select)
    * 返回  1 执行成功但无执行结果
    */
    int exec(const char* sql, std::vector<char**>* res, int* num_rows, int* num_fields);

private:
    std::mutex _lock;
    MYSQL* _mysql;
};

#endif
