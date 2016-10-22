#pragma once

#include <QObject>

class QSignalWrapperNoArgs : public QObject {
	Q_OBJECT
signals:
	void method(qint64);
};

class QSlotWrapperNoArgs : public QObject {
	Q_OBJECT
public:
	QSlotWrapperNoArgs(void (*func)(void*), void* data) {
        m_func = func;
        m_data = data;
    }
public slots:
	void method() {
	    m_func(m_data);
    }

private:
	void (*m_func)(void*);
	void* m_data;
};

