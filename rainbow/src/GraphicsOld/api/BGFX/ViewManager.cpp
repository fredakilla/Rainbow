#include "ViewManager.h"
#include <unordered_map>
#include <bgfx/bgfx.h>

namespace gameplay {

ViewManager::ViewManager() :
    _curentViewId(0)
{

}

unsigned short ViewManager::getCurrentViewId()
{
    return _curentViewId;
}

std::shared_ptr<View> ViewManager::getView(unsigned short id)
{
    if(_views.count(id)>0)
        return _views.at(id);
    else
        return nullptr;
}

void ViewManager::setViewOrder(std::vector<unsigned short>& viewIds)
{
    if(viewIds.size() == 0)
    {
        bgfx::setViewOrder();
    }
    else
    {
        bgfx::setViewOrder(viewIds[0], viewIds.size(), viewIds.data());
    }
}

void ViewManager::clearAll()
{
    for(auto it : _views)
    {
        std::shared_ptr<View> view = it.second;
        bgfx::resetView(view->id);
        view.reset();
    }

    _views.clear();
}



std::shared_ptr<View> ViewManager::createView(unsigned short id, Rectangle rectangle, View::ClearFlags clearFlags, unsigned int clearColor, float depth, unsigned char stencil)
{
    std::shared_ptr<View> view = nullptr;

    // search if the view id already exists in map
    size_t existingView = _views.count(id);
    if(existingView == 0)
    {
        // create a new view
        view = std::make_shared<View>();
    }
    else
    {
        // reset existing view
        view = _views.at(id);
        bgfx::resetView(id);
    }

    // setup view
    view->id = id;
    view->rectangle = rectangle;
    view->clearFlags = clearFlags;
    view->clearColor = clearColor;
    view->depth = depth;
    view->stencil = stencil;

    // pass to bgfx view states
    uint16_t flags = static_cast<uint16_t>(clearFlags);
    bgfx::setViewClear(id, flags, clearColor, depth, stencil);
    bgfx::setViewRect(id, rectangle.x, rectangle.y, rectangle.width, rectangle.height);

    // if this a new view, add to map
    if(existingView == 0)
        _views.insert(std::make_pair(id, view));

    return view;
}

void ViewManager::bind(unsigned short viewId)
{
    _curentViewId = viewId;

    std::shared_ptr<View> view = getView(viewId);
    if(!view)
        return;

    uint16_t clearFlags = static_cast<uint16_t>(view->clearFlags);

    bgfx::setViewClear(viewId,
                       clearFlags,
                       view->clearColor,
                       view->depth,
                       view->stencil);

    bgfx::setViewRect(viewId,
                      view->rectangle.x,
                      view->rectangle.y,
                      view->rectangle.width,
                      view->rectangle.height);
}

}
