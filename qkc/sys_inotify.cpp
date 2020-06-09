
#include <sys/inotify.h>
#include <windows.h>
#include <wintf/wobj.h>
#include "wobjs/INotifyMgr.h"
#include "wobjs/ObjMgr.h"

int inotify_init (void)
{
	qkc::INotifyMgr * mgr = new qkc::INotifyMgr();
	int index = mgr->Init();
	if (index < 0)
		delete mgr;

	return index;
}

int inotify_add_watch (int fd, const char *name, uint32_t mask)
{
	qkc::Object * obj = qkc::ObjMgr::Singleton().Get(fd);
	if (obj == NULL || obj->OType() != qkc::Object::kFileWatcher)
		return -1;

	qkc::INotifyMgr * mgr = (qkc::INotifyMgr *)obj;
	return mgr->Add(name, mask);
}

int inotify_rm_watch (int fd, uint32_t wd)
{
	qkc::Object * obj = qkc::ObjMgr::Singleton().Get(fd);
	if (obj == NULL || obj->OType() != qkc::Object::kFileWatcher)
		return -1;

	qkc::INotifyMgr * mgr = (qkc::INotifyMgr *)obj;
	return mgr->Del(wd);
}
