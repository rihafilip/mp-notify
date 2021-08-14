#include "notify.h"
#include <libnotify/notify.h>
#include <libnotify/notification.h>

Notify::Notify(const std::string &appName)
{
    notify_init( appName.c_str() );
}

Notify::~Notify()
{
    notify_uninit();
}

bool Notify::sendNotify ( const std::string & head, const std::string & body, const std::filesystem::path & image )
{
    NotifyNotification * notification = notify_notification_new( head.c_str(), body.c_str(), image.c_str() );
    notify_notification_set_urgency( notification, NotifyUrgency::NOTIFY_URGENCY_NORMAL );
    notify_notification_set_timeout(notification, 10000 );
    //notify_notification_set_image_from_pixbuf();

    return notify_notification_show( notification, nullptr );
}

