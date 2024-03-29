/* ��������� ������������ �����. ����� ��� �������� ����������� ������
   (�������� ���������� ��������� �������) */
#ifndef DATABOX_H
#define DATABOX_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QReadWriteLock>
#include <QDateTime>
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
template<typename T> class DataBox
{
public:
    explicit DataBox();
    explicit DataBox(const DataBox<T>&);
    DataBox<T>& operator =(const DataBox<T>&);
    const T&         data()       const {return _data;}
    T&               data()             {return _data;}
    QReadWriteLock&  lock()             {return _lock;}
    const QDateTime& timeMarker() const {return _marker;}
    void             setTimeMarker(const QDateTime &t) {_marker = t;}
    void             refreshTime() {this->setTimeMarker(QDateTime::currentDateTimeUtc());}
private:
    T              _data;   //����� ����� ������ (� �.�. ������������), � �������� ��������� ���������� �������� ������������
    QReadWriteLock _lock;   //mutex, ������� ����� �������� ��� ����������� ������� � ������ (� ����� �����������!!!)
    QDateTime      _marker; //����� �������
};
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
template<typename T>
DataBox<T>::DataBox()
{
    this->refreshTime();
}
/////////////////////////////////////////////////////////////////////////////////////
template<typename T>
DataBox<T>::DataBox(const DataBox<T> &other)
{
    this->_data = other._data;
    this->_marker = other._marker;
}
/////////////////////////////////////////////////////////////////////////////////////
template<typename T>
DataBox<T>& DataBox<T>::operator =(const DataBox<T> &other)
{
    if(this != &other)
    {
        this->_data = other._data;
        this->_marker = other._marker;
    }
    return *this;
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // DATABOX_H
