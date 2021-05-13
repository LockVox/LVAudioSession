#ifndef CONFIG_H
#define CONFIG_H
#include <QObject>
#if defined MAKE_TEST_LIB
    #define TEST_LIB_EXPORT Q_DECL_EXPORT
#else
    #define TEST_LIB_EXPORT Q_DECL_IMPORT
#endif


#endif // CONFIG_H
