/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LxQt - a lightweight, Qt based, desktop toolset
 * http://razor-qt.org, http://lxde.org/
 *
 * Copyright: 2010-2011 LxQt team
 * Authors:
 *   Petr Vanek <petr@scribus.info>
 *
 * This program or library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 *
 * END_COMMON_COPYRIGHT_HEADER */

#include <lxqt/lxqtapplication.h>
#include <QtGui/QIcon>
#include <unistd.h>

#include "lxqtmodman.h"
#include "sessiondbusadaptor.h"
#include "lxqttranslate.h"

/**
* @file main.cpp
* @author Christopher "VdoP" Regali
* @brief just starts the sub-apps and in future maybe saves the windowstates

lxqt-session can be called as is (without any argument) - it will start
"failback" session (sessionf.conf). When there will be used -c foo argument
lxqt-session -c foo
it will use foo.conf. Currently there are launchers for windowmanagers:
session-openbox.conf
session-eggwm.conf
*/

/**
* @brief our main function doing the loading
*/
int main(int argc, char **argv)
{
    LxQt::Application app(argc, argv);
    //qDebug() << "LxQt Session start";
    app.setQuitOnLastWindowClosed(false);

    TRANSLATE_APP;

    char* session = NULL;
    char* winmanager = NULL;
    int c;
    while ((c = getopt (argc, argv, "c:w:")) != -1)
    {
        if (c == 'c')
        {
            session = optarg;
            break;
        }
        else if (c == 'w')
        {
            winmanager = optarg;
            break;
        }
    }
    
#ifdef PATH_PREPEND
    // PATH for out own bundled XDG tools
    lxqt_setenv_prepend("PATH", PATH_PREPEND);
#endif // PATH_PREPEND

    // special variable for LxQt environment menu
    lxqt_setenv("XDG_MENU_PREFIX", "lxqt-");

    LxQtModuleManager modman(session, winmanager);
    new SessionDBusAdaptor(&modman);
    // connect to D-Bus and register as an object:
    QDBusConnection::sessionBus().registerService("org.lxqt.session");
    QDBusConnection::sessionBus().registerObject("/LxQtSession", &modman);
    return app.exec();
}
