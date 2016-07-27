#ifndef QRWIDGETS_GLOBAL_H
#define QRWIDGETS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QRWIDGETS_LIBRARY)
#  define QRWIDGETSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QRWIDGETSSHARED_EXPORT Q_DECL_IMPORT
#endif

//  qrwidgets namespace
#define NS_QRWIDGETS_BEGIN \
    namespace Qters { \
    namespace QrWidgets {
#define NS_QRWIDGETS_END \
    } \
    }
#define USING_NS_QRWIDGETS    using namespace Qters::QrWidgets
#define NS_QRWIDGETS  ::Qters::QrWidgets

#endif // QRWIDGETS_GLOBAL_H
