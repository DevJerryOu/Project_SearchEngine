#include "../../include/framework/MyLog.h"
MyLog *MyLog::getInstance()
{
    if (_pLog == nullptr)
    {
        _pLog = new MyLog();
    }
    return _pLog;
}
void MyLog::destroy()
{
    if (_pLog)
    {
        delete _pLog;
        _pLog = nullptr;
    }
}
void MyLog::writeInLog(Configuration &conf, const string &str)
{
    getDataTime();
    ofstream ofs((conf)["mylog"], ios::app);
    if (!ofs)
    {
        cout << "open mylog.dat fail!" << endl;
        exit(0);
    }
    ofs << _time << str << endl
        << endl;
    ofs.close();
}
MyLog::MyLog()
{
}
void MyLog::getDataTime()
{
    time_t tm;
    tm = time(nullptr);
    _time = ctime(&tm);
    // cout << "_time:" << _time << endl;
}
MyLog *MyLog::_pLog = nullptr;