/********************************************************************
 *             a  A
 *            AM\/MA
 *           (MA:MMD
 *            :: VD
 *           ::  º
 *          ::
 *         ::   **      .A$MMMMND   AMMMD     AMMM6    MMMM  MMMM6
 +       6::Z. TMMM    MMMMMMMMMDA   VMMMD   AMMM6     MMMMMMMMM6
 *      6M:AMMJMMOD     V     MMMA    VMMMD AMMM6      MMMMMMM6
 *      ::  TMMTMC         ___MMMM     VMMMMMMM6       MMMM
 *     MMM  TMMMTTM,     AMMMMMMMM      VMMMMM6        MMMM
 *    :: MM TMMTMMMD    MMMMMMMMMM       MMMMMM        MMMM
 *   ::   MMMTTMMM6    MMMMMMMMMMM      AMMMMMMD       MMMM
 *  :.     MMMMMM6    MMMM    MMMM     AMMMMMMMMD      MMMM
 *         TTMMT      MMMM    MMMM    AMMM6  MMMMD     MMMM
 *        TMMMM8       MMMMMMMMMMM   AMMM6    MMMMD    MMMM
 *       TMMMMMM$       MMMM6 MMMM  AMMM6      MMMMD   MMMM
 *      TMMM MMMM
 *     TMMM  .MMM
 *     TMM   .MMD       ARBITRARY·······XML········RENDERING
 *     TMM    MMA       ====================================
 *     TMN    MM
 *      MN    ZM
 *            MM,
 *
 *
 *      COPYRIGHT: ©2012 - All Rights Reserved
 *
 *      LICENSE: see LICENSE file
 *
 *      WEB: http://axr.vg
 *
 *      THIS CODE AND INFORMATION ARE PROVIDED "AS IS"
 *      WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
 *      OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *      IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR
 *      FITNESS FOR A PARTICULAR PURPOSE.
 *
 ********************************************************************/

#ifndef HSSCIRCLE_H
#define HSSCIRCLE_H

#include "HSSShape.h"

namespace AXR
{

    /**
     *  @brief Shape object, representing the HSS object \@circle.
     */
    class HSSCircle : public HSSShape
    {
    public:
        typedef boost::shared_ptr<HSSCircle> p;

        /**
         *  Constructor for HSSCircle objects. Do not call directly, use clone() instead.
         */
        HSSCircle();

        /**
         *  Copy constructor for HSSCircle objects
         */
        HSSCircle(const HSSCircle & orig);

        /**
         *  Clones an instance of HSSCircle and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSCircle.
         */
        p clone() const;

        /**
         *  Destructor for this class.
         */
        virtual ~HSSCircle();

        virtual std::string toString();
        virtual std::string defaultObjectType();
        virtual std::string defaultObjectType(std::string property);
        virtual bool isKeyword(std::string value, std::string property);

        /**
         *  Call this method when you need to draw shape into the cairo context. The coordinates are used
         *  to define the bounding box into which the shape will be drawn.
         *  @param cairo    A regular pointer to a cairo handle representing the drawing context.
         *  @param x        The horizontal position of the shape.
         *  @param y        The vertical position of the shape.
         *  @param width    The width of the shape.
         *  @param height   The height of the shape.
         */
        virtual void draw(cairo_t * cairo, HSSUnit x, HSSUnit y, HSSUnit width, HSSUnit height);

    private:
        HSSClonable::p cloneImpl() const;
    };
}

#endif
