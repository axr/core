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
 *      AUTHORS: see AUTHORS file
 *
 *      COPYRIGHT: ©2019 - All Rights Reserved
 *
 *      LICENSE: see LICENSE file
 *
 *      WEB: http://axrproject.org
 *
 *      THIS CODE AND INFORMATION ARE PROVIDED "AS IS"
 *      WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
 *      OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *      IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR
 *      FITNESS FOR A PARTICULAR PURPOSE.
 *
 ********************************************************************/

#ifndef HSSPATH_H
#define HSSPATH_H

namespace AXR
{
    class HSSPathPrivate;
    class HSSPathCommand;

    class HSSPath
    {
    public:
        HSSPath();
        virtual ~HSSPath();
        void addCommand(QSharedPointer<HSSPathCommand> command);
        const std::vector<QSharedPointer<HSSPathCommand> > getCommands() const;

        void moveTo(HSSUnit x, HSSUnit y);
        void lineTo(HSSUnit x, HSSUnit y);
        void arcTo(HSSRect, HSSUnit angle, HSSUnit sweepLength);
        void subtract(QSharedPointer<HSSPath> otherPath);
        void closeSubpath();
        void addRect(HSSUnit x, HSSUnit y, HSSUnit width, HSSUnit height);
        void addEllipse(HSSRect rect);
        void addPolygon(std::vector<HSSPoint> points);
        
    private:
        HSSPathPrivate * d;
    };
}


#endif /* HSSPATH_H */
