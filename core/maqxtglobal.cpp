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

#include "maqxtglobal.h"

/*!
    \headerfile <MAQxtGlobal>
    \title Global MAQxt Declarations
    \inmodule MAQxtCore

    \brief The <MAQxtGlobal> header provides basic declarations and
    is included by all other MAQxt headers.
 */

/*!
    \macro MAQXT_VERSION
    \relates <MAQxtGlobal>

    This macro expands a numeric value of the form 0xMMNNPP (MM =
    major, NN = minor, PP = patch) that specifies MAQxt's version
    number. For example, if you compile your application against MAQxt
    0.4.0, the MAQXT_VERSION macro will expand to 0x000400.

    You can use MAQXT_VERSION to use the latest Qt features where
    available. For example:
    \code
    #if MAQXT_VERSION >= 0x000400
        qxtTabWidget->setTabMovementMode(MAQxtTabWidget::InPlaceMovement);
    #endif
    \endcode

    \sa MAQXT_VERSION_STR, qxtVersion()
 */

/*!
    \macro MAQXT_VERSION_STR
    \relates <MAQxtGlobal>

    This macro expands to a string that specifies MAQxt's version number
    (for example, "0.4.0"). This is the version against which the
    application is compiled.

    \sa qxtVersion(), MAQXT_VERSION
 */

/*!
    \relates <MAQxtGlobal>

    Returns the version number of MAQxt at run-time as a string (for
    example, "0.4.0"). This may be a different version than the
    version the application was compiled against.

    \sa MAQXT_VERSION_STR
 */
const char* qxtVersion()
{
    return MAQXT_VERSION_STR;
}

/*!
\headerfile <MAQxtPimpl>
\title The MAQxt private implementation
\inmodule MAQxtCore

\brief The <MAQxtPimpl> header provides tools for hiding
details of a class.

Application code generally doesn't have to be concerned about hiding its
implementation details, but when writing library code it is important to
maintain a constant interface, both source and binary. Maintaining a constant
source interface is easy enough, but keeping the binary interface constant
means moving implementation details into a private class. The PIMPL, or
d-pointer, idiom is a common method of implementing this separation. MAQxtPimpl
offers a convenient way to connect the public and private sides of your class.

\section1 Getting Started
Before you declare the public class, you need to make a forward declaration
of the private class. The private class must have the same name as the public
class, followed by the word Private. For example, a class named MyTest would
declare the private class with:
\code
class MyTestPrivate;
\endcode

\section1 The Public Class
Generally, you shouldn't keep any data members in the public class without a
good reason. Functions that are part of the public interface should be declared
in the public class, and functions that need to be available to subclasses (for
calling or overriding) should be in the protected section of the public class.
To connect the private class to the public class, include the
MAQXT_DECLARE_PRIVATE macro in the private section of the public class. In the
example above, the private class is connected as follows:
\code
private:
    MAQXT_DECLARE_PRIVATE(MyTest)
\endcode

Additionally, you must include the MAQXT_INIT_PRIVATE macro in the public class's
constructor. Continuing with the MyTest example, your constructor might look
like this:
\code
MyTest::MyTest() {
    // initialization
    MAQXT_INIT_PRIVATE(MyTest);
}
\endcode

\section1 The Private Class
As mentioned above, data members should usually be kept in the private class.
This allows the memory layout of the private class to change without breaking
binary compatibility for the public class. Functions that exist only as
implementation details, or functions that need access to private data members,
should be implemented here.

To define the private class, inherit from the template MAQxtPrivate class, and
include the MAQXT_DECLARE_PUBLIC macro in its public section. The template
parameter should be the name of the public class. For example:
\code
class MyTestPrivate : public MAQxtPrivate<MyTest> {
public:
    MyTestPrivate();
    MAQXT_DECLARE_PUBLIC(MyTest)
};
\endcode

\section1 Accessing Private Members
Use the qxt_d() function (actually a function-like object) from functions in
the public class to access the private class. Similarly, functions in the
private class can invoke functions in the public class by using the qxt_p()
function (this one's actually a function).

For example, assume that MyTest has methods named getFoobar and doBaz(),
and MyTestPrivate has a member named foobar and a method named doQuux().
The code might resemble this example:
\code
int MyTest::getFoobar() {
    return qxt_d().foobar;
}

void MyTestPrivate::doQuux() {
    qxt_p().doBaz(foobar);
}
\endcode
*/

/*! 
 * \macro MAQXT_DECLARE_PRIVATE(PUB)
 * \relates <MAQxtPimpl>
 * Declares that a public class has a related private class.
 *
 * This shuold be put in the private section of the public class. The parameter is the name of the public class.
 */

/*!
 * \macro MAQXT_DECLARE_PUBLIC(PUB)
 * \relates <MAQxtPimpl>
 * Declares that a private class has a related public class.
 *
 * This may be put anywhere in the declaration of the private class. The parameter is the name of the public class.
 */

/*!
 * \macro MAQXT_INIT_PRIVATE(PUB)
 * \relates <MAQxtPimpl>
 * Initializes resources owned by the private class.
 *
 * This should be called from the public class's constructor,
 * before qxt_d() is used for the first time. The parameter is the name of the public class.
 */

/*!
 * \macro MAQXT_D(PUB)
 * \relates <MAQxtPimpl>
 * Returns a reference in the current scope named "d" to the private class.
 *
 * This function is only available in a class using \a MAQXT_DECLARE_PRIVATE.
 */

/*!
 * \macro MAQXT_P(PUB)
 * \relates <MAQxtPimpl>
 * Creates a reference in the current scope named "q" to the public class.
 *
 * This macro only works in a class using \a MAQXT_DECLARE_PUBLIC.
 */

/*!
 * \fn MAQxtPrivate<PUB>& PUB::qxt_d()
 * \relates <MAQxtPimpl>
 * Returns a reference to the private class.
 *
 * This function is only available in a class using \a MAQXT_DECLARE_PRIVATE.
 */

/*!
 * \fn const MAQxtPrivate<PUB>& PUB::qxt_d() const
 * \relates <MAQxtPimpl>
 * Returns a const reference to the private class.
 *
 * This function is only available in a class using \a MAQXT_DECLARE_PRIVATE.
 * This overload will be automatically used in const functions.
 */

/*!
 * \fn PUB& MAQxtPrivate::qxt_p()
 * \relates <MAQxtPimpl>
 * Returns a reference to the public class.
 *
 * This function is only available in a class using \a MAQXT_DECLARE_PUBLIC.
 */

/*!
 * \fn const PUB& MAQxtPrivate::qxt_p() const
 * \relates <MAQxtPimpl>
 * Returns a const reference to the public class.
 *
 * This function is only available in a class using \a MAQXT_DECLARE_PUBLIC.
 * This overload will be automatically used in const functions.
 */
