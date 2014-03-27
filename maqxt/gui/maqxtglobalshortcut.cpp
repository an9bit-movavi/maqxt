/****************************************************************************
 **
 ** Copyright (C) Qxt Foundation. Some rights reserved.
 **
 ** This file is part of the QxtGui module of the Qxt library.
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
#include "maqxtglobalshortcut.h"
#include "maqxtglobalshortcut_p.h"
#include <QAbstractEventDispatcher>
#include <QtDebug>

bool MAQxtGlobalShortcutPrivate::error = false;
#ifndef Q_WS_MAC
int MAQxtGlobalShortcutPrivate::ref = 0;
QAbstractEventDispatcher::EventFilter MAQxtGlobalShortcutPrivate::prevEventFilter = 0;
#endif // Q_WS_MAC
QHash<QPair<quint32, quint32>, MAQxtGlobalShortcut*> MAQxtGlobalShortcutPrivate::shortcuts;

MAQxtGlobalShortcutPrivate::MAQxtGlobalShortcutPrivate() : enabled(true), key(Qt::Key(0)), mods(Qt::NoModifier)
{
#ifndef Q_WS_MAC
    if (!ref++)
        prevEventFilter = QAbstractEventDispatcher::instance()->setEventFilter(eventFilter);
#endif // Q_WS_MAC
}

MAQxtGlobalShortcutPrivate::~MAQxtGlobalShortcutPrivate()
{
#ifndef Q_WS_MAC
    if (!--ref)
        QAbstractEventDispatcher::instance()->setEventFilter(prevEventFilter);
#endif // Q_WS_MAC
}

bool MAQxtGlobalShortcutPrivate::setShortcut(const QKeySequence& shortcut)
{
    Qt::KeyboardModifiers allMods = Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier | Qt::MetaModifier;
    key = shortcut.isEmpty() ? Qt::Key(0) : Qt::Key((shortcut[0] ^ allMods) & shortcut[0]);
    mods = shortcut.isEmpty() ? Qt::KeyboardModifiers(0) : Qt::KeyboardModifiers(shortcut[0] & allMods);
    const quint32 nativeKey = nativeKeycode(key);
    const quint32 nativeMods = nativeModifiers(mods);
    const bool res = registerShortcut(nativeKey, nativeMods);
    if (res)
        shortcuts.insert(qMakePair(nativeKey, nativeMods), &qxt_p());
    else
        qWarning() << "MAQxtGlobalShortcut failed to register:" << QKeySequence(key + mods).toString();
    return res;
}

bool MAQxtGlobalShortcutPrivate::unsetShortcut()
{
    bool res = false;
    const quint32 nativeKey = nativeKeycode(key);
    const quint32 nativeMods = nativeModifiers(mods);
    if (shortcuts.value(qMakePair(nativeKey, nativeMods)) == &qxt_p())
        res = unregisterShortcut(nativeKey, nativeMods);
    if (res)
        shortcuts.remove(qMakePair(nativeKey, nativeMods));
    else
        qWarning() << "MAQxtGlobalShortcut failed to unregister:" << QKeySequence(key + mods).toString();
    key = Qt::Key(0);
    mods = Qt::KeyboardModifiers(0);
    return res;
}

void MAQxtGlobalShortcutPrivate::activateShortcut(quint32 nativeKey, quint32 nativeMods)
{
    MAQxtGlobalShortcut* shortcut = shortcuts.value(qMakePair(nativeKey, nativeMods));
    if (shortcut && shortcut->isEnabled())
        emit shortcut->activated();
}

/*!
    \class MAQxtGlobalShortcut
    \inmodule MAQxtGui
    \brief The MAQxtGlobalShortcut class provides a global shortcut aka "hotkey".

    A global shortcut triggers even if the application is not active. This
    makes it easy to implement applications that react to certain shortcuts
    still if some other application is active or if the application is for
    example minimized to the system tray.

    Example usage:
    \code
    MAQxtGlobalShortcut* shortcut = new MAQxtGlobalShortcut(window);
    connect(shortcut, SIGNAL(activated()), window, SLOT(toggleVisibility()));
    shortcut->setShortcut(QKeySequence("Ctrl+Shift+F12"));
    \endcode

    \bold {Note:} Since MAQxt 0.6 MAQxtGlobalShortcut no more requires MAQxtApplication.
 */

/*!
    \fn MAQxtGlobalShortcut::activated()

    This signal is emitted when the user types the shortcut's key sequence.

    \sa shortcut
 */

/*!
    Constructs a new MAQxtGlobalShortcut with \a parent.
 */
MAQxtGlobalShortcut::MAQxtGlobalShortcut(QObject* parent)
        : QObject(parent)
{
    MAQXT_INIT_PRIVATE(MAQxtGlobalShortcut);
}

/*!
    Constructs a new MAQxtGlobalShortcut with \a shortcut and \a parent.
 */
MAQxtGlobalShortcut::MAQxtGlobalShortcut(const QKeySequence& shortcut, QObject* parent)
        : QObject(parent)
{
    MAQXT_INIT_PRIVATE(MAQxtGlobalShortcut);
    setShortcut(shortcut);
}

/*!
    Destructs the MAQxtGlobalShortcut.
 */
MAQxtGlobalShortcut::~MAQxtGlobalShortcut()
{
    if (qxt_d().key != 0)
        qxt_d().unsetShortcut();
}

/*!
    \property MAQxtGlobalShortcut::shortcut
    \brief the shortcut key sequence

    \bold {Note:} Notice that corresponding key press and release events are not
    delivered for registered global shortcuts even if they are disabled.
    Also, comma separated key sequences are not supported.
    Only the first part is used:

    \code
    qxtShortcut->setShortcut(QKeySequence("Ctrl+Alt+A,Ctrl+Alt+B"));
    Q_ASSERT(qxtShortcut->shortcut() == QKeySequence("Ctrl+Alt+A"));
    \endcode
 */
QKeySequence MAQxtGlobalShortcut::shortcut() const
{
    return QKeySequence(qxt_d().key | qxt_d().mods);
}

bool MAQxtGlobalShortcut::setShortcut(const QKeySequence& shortcut)
{
    if (qxt_d().key != 0)
        qxt_d().unsetShortcut();
    return qxt_d().setShortcut(shortcut);
}

/*!
    \property MAQxtGlobalShortcut::enabled
    \brief whether the shortcut is enabled

    A disabled shortcut does not get activated.

    The default value is \c true.

    \sa setDisabled()
 */
bool MAQxtGlobalShortcut::isEnabled() const
{
    return qxt_d().enabled;
}

void MAQxtGlobalShortcut::setEnabled(bool enabled)
{
    qxt_d().enabled = enabled;
}

/*!
    Sets the shortcut \a disabled.

    \sa enabled
 */
void MAQxtGlobalShortcut::setDisabled(bool disabled)
{
    qxt_d().enabled = !disabled;
}
