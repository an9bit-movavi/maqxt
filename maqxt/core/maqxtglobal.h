/****************************************************************************
 **
 ** Copyright (C) Qxt Foundation. Some rights reserved.
 **
 ** This file is part of the QxtCore module of the Qxt library.
 **
 ** This library is free software; you can redistribute it and/or modify it
 ** under the terms of the Common Public License, version 1.0, as published
 ** by IBM, and/or under the terms of the GNU Lesser General Public License,
 ** version 2.1, as published by the Free Software Foundation.
 **
 ** This file is provided "AS IS", without WARRANTIES OR CONDITIONS OF ANY
 ** KIND, EITHER EXPRESS OR IMPLIED INCLUDING, WITHOUT LIMITATION, ANY
 ** WARRANTIES OR CONDITIONS OF TITLE, NON-INFRINGEMENT, MERCHANTABILITY OR
 ** FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** You should have received a copy of the CPL and the LGPL along with this
 ** file. See the LICENSE file and the cpl1.0.txt/lgpl-2.1.txt files
 ** included with the source distribution for more information.
 ** If you did not receive a copy of the licenses, contact the Qxt Foundation.
 **
 ** <http://libqxt.org>  <foundation@libqxt.org>
 **
 ****************************************************************************/

#ifndef MAQXTGLOBAL_H
#define MAQXTGLOBAL_H

#include <QtGlobal>

#define MAQXT_VERSION 0x000602
#define MAQXT_VERSION_STR "0.6.2"

//--------------------------global macros------------------------------

#ifndef MAQXT_NO_MACROS

#endif // MAQXT_NO_MACROS

//--------------------------export macros------------------------------

#define MAQXT_DLLEXPORT DO_NOT_USE_THIS_ANYMORE

#if !defined(MAQXT_STATIC)
#    if defined(BUILD_MAQXT_CORE)
#        define MAQXT_CORE_EXPORT Q_DECL_EXPORT
#    else
#        define MAQXT_CORE_EXPORT Q_DECL_IMPORT
#    endif
#else
#    define MAQXT_CORE_EXPORT
#endif // BUILD_MAQXT_CORE
 
#if !defined(MAQXT_STATIC)
#    if defined(BUILD_MAQXT_GUI)
#        define MAQXT_GUI_EXPORT Q_DECL_EXPORT
#    else
#        define MAQXT_GUI_EXPORT Q_DECL_IMPORT
#    endif
#else
#    define MAQXT_GUI_EXPORT
#endif // BUILD_MAQXT_GUI
 
#if !defined(MAQXT_STATIC)
#    if defined(BUILD_MAQXT_NETWORK)
#        define MAQXT_NETWORK_EXPORT Q_DECL_EXPORT
#    else
#        define MAQXT_NETWORK_EXPORT Q_DECL_IMPORT
#    endif
#else
#    define MAQXT_NETWORK_EXPORT
#endif // BUILD_MAQXT_NETWORK
 
#if !defined(MAQXT_STATIC)
#    if defined(BUILD_MAQXT_SQL)
#        define MAQXT_SQL_EXPORT Q_DECL_EXPORT
#    else
#        define MAQXT_SQL_EXPORT Q_DECL_IMPORT
#    endif
#else
#    define MAQXT_SQL_EXPORT
#endif // BUILD_MAQXT_SQL
 
#if !defined(MAQXT_STATIC)
#    if defined(BUILD_MAQXT_WEB)
#        define MAQXT_WEB_EXPORT Q_DECL_EXPORT
#    else
#        define MAQXT_WEB_EXPORT Q_DECL_IMPORT
#    endif
#else
#    define MAQXT_WEB_EXPORT
#endif // BUILD_MAQXT_WEB
 
#if !defined(MAQXT_STATIC)
#    if defined(BUILD_MAQXT_BERKELEY)
#        define MAQXT_BERKELEY_EXPORT Q_DECL_EXPORT
#    else
#        define MAQXT_BERKELEY_EXPORT Q_DECL_IMPORT
#    endif
#else
#    define MAQXT_BERKELEY_EXPORT
#endif // BUILD_MAQXT_BERKELEY

#if !defined(MAQXT_STATIC)
#    if defined(BUILD_MAQXT_ZEROCONF)
#        define MAQXT_ZEROCONF_EXPORT Q_DECL_EXPORT
#    else
#        define MAQXT_ZEROCONF_EXPORT Q_DECL_IMPORT
#    endif
#else
#    define MAQXT_ZEROCONF_EXPORT
#endif // MAQXT_ZEROCONF_EXPORT

#if defined BUILD_MAQXT_CORE || defined BUILD_MAQXT_GUI || defined  BUILD_MAQXT_SQL || defined BUILD_MAQXT_NETWORK || defined BUILD_MAQXT_WEB || defined BUILD_MAQXT_BERKELEY || defined BUILD_MAQXT_ZEROCONF
#   define BUILD_MAQXT
#endif

MAQXT_CORE_EXPORT const char* qxtVersion();

#ifndef QT_BEGIN_NAMESPACE
#define QT_BEGIN_NAMESPACE
#endif

#ifndef QT_END_NAMESPACE
#define QT_END_NAMESPACE
#endif

#ifndef QT_FORWARD_DECLARE_CLASS
#define QT_FORWARD_DECLARE_CLASS(Class) class Class;
#endif

/****************************************************************************
** This file is derived from code bearing the following notice:
** The sole author of this file, Adam Higerd, has explicitly disclaimed all
** copyright interest and protection for the content within. This file has
** been placed in the public domain according to United States copyright
** statute and case law. In jurisdictions where this public domain dedication
** is not legally recognized, anyone who receives a copy of this file is
** permitted to use, modify, duplicate, and redistribute this file, in whole
** or in part, with no restrictions or conditions. In these jurisdictions,
** this file shall be copyright (C) 2006-2008 by Adam Higerd.
****************************************************************************/

#define MAQXT_DECLARE_PRIVATE(PUB) friend class PUB##Private; MAQxtPrivateInterface<PUB, PUB##Private> qxt_d;
#define MAQXT_DECLARE_PUBLIC(PUB) friend class PUB;
#define MAQXT_INIT_PRIVATE(PUB) qxt_d.setPublic(this);
#define MAQXT_D(PUB) PUB##Private& d = qxt_d()
#define MAQXT_P(PUB) PUB& p = qxt_p()

template <typename PUB>
class MAQxtPrivate
{
public:
    virtual ~MAQxtPrivate()
    {}
    inline void MAQXT_setPublic(PUB* pub)
    {
        qxt_p_ptr = pub;
    }

protected:
    inline PUB& qxt_p()
    {
        return *qxt_p_ptr;
    }
    inline const PUB& qxt_p() const
    {
        return *qxt_p_ptr;
    }

private:
    PUB* qxt_p_ptr;
};

template <typename PUB, typename PVT>
class MAQxtPrivateInterface
{
    friend class MAQxtPrivate<PUB>;
public:
    MAQxtPrivateInterface()
    {
        pvt = new PVT;
    }
    ~MAQxtPrivateInterface()
    {
        delete pvt;
    }

    inline void setPublic(PUB* pub)
    {
        pvt->MAQXT_setPublic(pub);
    }
    inline PVT& operator()()
    {
        return *static_cast<PVT*>(pvt);
    }
    inline const PVT& operator()() const
    {
        return *static_cast<PVT*>(pvt);
    }
private:
    MAQxtPrivateInterface(const MAQxtPrivateInterface&) { }
    MAQxtPrivateInterface& operator=(const MAQxtPrivateInterface&) { }
    MAQxtPrivate<PUB>* pvt;
};

#endif // MAQXT_GLOBAL
