#ifndef VIEWMANAGER_H
#define VIEWMANAGER_H

#include "Base.h"
#include "Rectangle.h"
#include <vector>

namespace gameplay {


/**
 * View is a sorting mechanism. View represent bucket of draw and compute calls.
 */
struct View
{
public:

    /**
     * The ClearFlags enum
     */
    enum class ClearFlags : unsigned short
    {
        NONE    = 0x0000, // BGFX_CLEAR_NONE
        COLOR   = 0x0001, // BGFX_CLEAR_COLOR
        DEPTH   = 0x0002, // BGFX_CLEAR_DEPTH
        STENCIL = 0x0004, // BGFX_CLEAR_STENCIL
        COLOR_DEPTH = COLOR | DEPTH,
        COLOR_STENCIL = COLOR | STENCIL,
        DEPTH_STENCIL = DEPTH | STENCIL,
        COLOR_DEPTH_STENCIL = COLOR | DEPTH | STENCIL
    };

    /**
     * The SortingMode enum
     */
    enum class SortingMode
    {
        DEFAULT,            //!< Default sort order.
        SEQUENTIAL,         //!< Sort in the same order in which submit calls were called.
        DEPTH_ASCENDING,    //!< Sort draw call depth in ascending order.
        DEPTH_DESCENDING,   //!< Sort draw call depth in descending order.
    };

    /**
     * Constructor
     */
    View() :
        id(0)
      , rectangle(Rectangle())
      , clearFlags(ClearFlags::NONE)
      , clearColor(0x00000000)
      , depth(1.0f)
      , stencil(0)
      , name("")
    {

    }

    // properties
    unsigned short id;
    Rectangle rectangle;
    ClearFlags clearFlags;
    unsigned int clearColor;
    float depth;
    unsigned char stencil;
    std::string name;
};




class ViewManager
{
public:

    /**
     * Constructor
     */
    ViewManager();

    /**
     * Create a view.
     * The view is added to the list of views.
     *
     * @param id The id of the view
     * @param rectangle The origin and size of the view.
     * @param clearFlags The clear flags - see ClearFlags enum.
     * @param color The clear color.
     * @param depth The depth value.
     * @param stencil The stencil value.
     * @return
     */
    std::shared_ptr<View> createView(unsigned short id, Rectangle rectangle, View::ClearFlags clearFlags,
                        unsigned int color = 0x00000000, float depth = 1.0f, unsigned char stencil = 0);

    void bind(unsigned short viewId);
    unsigned short getCurrentViewId();
    std::shared_ptr<View> getView(unsigned short id);

    /**
     * @brief View reordering
     * @param views View remap id table. Passing empty vector will reset view ids to default state.
     */
    void setViewOrder(std::vector<unsigned short>& viewIds);

    /**
     * reset to default all views and clear cache
     */
    void clearAll();

private:

    std::unordered_map<unsigned short, std::shared_ptr<View>> _views;
    unsigned short _curentViewId;

};

}

#endif // VIEW_H
