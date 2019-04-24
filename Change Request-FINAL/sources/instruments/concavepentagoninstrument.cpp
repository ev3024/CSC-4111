#include "concavepentagoninstrument.h"
#include "../imagearea.h"
#include "../datasingleton.h"
#include <cmath>

#include <QPen>
#include <QPainter>

ConcavePentagonInstrument::ConcavePentagonInstrument(QObject *parent) :
    AbstractInstrument(parent)
{

}
    void ConcavePentagonInstrument::mousePressEvent(QMouseEvent *event, ImageArea &imageArea)
    {
		if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton)
		{
			mStartPoint = mEndPoint = event->pos();
			imageArea.setIsPaint(true);
			mImageCopy = *imageArea.getImage();
			makeUndoCommand(imageArea);
		}
    }

    void ConcavePentagonInstrument::mouseMoveEvent(QMouseEvent *event, ImageArea &imageArea)
    {
		if (imageArea.isPaint())
		{
			mEndPoint = event->pos();
			imageArea.setImage(mImageCopy);
			if (event->buttons() & Qt::LeftButton)
			{
				paint(imageArea, false);
			}
			else if (event->buttons() & Qt::RightButton)
			{
				paint(imageArea, true);
			}
		}
    }

    void ConcavePentagonInstrument::mouseReleaseEvent(QMouseEvent *event, ImageArea &imageArea)
    {
		if (imageArea.isPaint())
		{
			imageArea.setImage(mImageCopy);
			if (event->button() == Qt::LeftButton)
			{
				paint(imageArea, false);
			}
			else if (event->button() == Qt::RightButton)
			{
				paint(imageArea, true);
			}
			imageArea.setIsPaint(false);
		}
    }

    void ConcavePentagonInstrument::paint(ImageArea &imageArea, bool isSecondaryColor, bool additionalFlag)
    {
		QPainter painter(imageArea.getImage());
		painter.setPen(QPen(DataSingleton::Instance()->getPrimaryColor(),
			DataSingleton::Instance()->getPenSize() * imageArea.getZoomFactor(),
			Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
		if (isSecondaryColor)
		{
			painter.setBrush(QBrush(DataSingleton::Instance()->getSecondaryColor()));
		}
		if (mStartPoint != mEndPoint)
		{

			//Added in functionality for a Concave Pentagon
			QPolygon poly;

			int absXSize, absYSize, xSize, ySize;

			xSize = mStartPoint.x() - mEndPoint.x();
			ySize = mStartPoint.y() - mEndPoint.y();

			absXSize = abs(xSize);
			absYSize = abs(ySize);

			if (xSize > 0 && ySize > 0)
			{
				poly << QPoint(mStartPoint.x() - absXSize, mStartPoint.y() - absYSize);
				poly << QPoint(mStartPoint.x() - absXSize, mStartPoint.y());
				poly << QPoint(mStartPoint.x(), mStartPoint.y() + absYSize);
				poly << QPoint(mStartPoint.x(), mStartPoint.y());
				poly << QPoint(mStartPoint.x() + absXSize, mStartPoint.y());
			}
			else if (xSize > 0 && ySize < 0)
			{
				poly << QPoint(mStartPoint.x() - absXSize, mStartPoint.y() + absYSize);
				poly << QPoint(mStartPoint.x() - absXSize, mStartPoint.y());
				poly << QPoint(mStartPoint.x(), mStartPoint.y() - absYSize);
				poly << QPoint(mStartPoint.x(), mStartPoint.y());
				poly << QPoint(mStartPoint.x() + absXSize, mStartPoint.y());
			}
			else if (xSize < 0 && ySize < 0)
			{
				poly << QPoint(mStartPoint.x() + absXSize, mStartPoint.y() + absYSize);
				poly << QPoint(mStartPoint.x() + absXSize, mStartPoint.y());
				poly << QPoint(mStartPoint.x(), mStartPoint.y() - absYSize);
				poly << QPoint(mStartPoint.x(), mStartPoint.y());
				poly << QPoint(mStartPoint.x() - absXSize, mStartPoint.y());
			}
			else
			{
				poly << QPoint(mStartPoint.x() + absXSize, mStartPoint.y() - absYSize);
				poly << QPoint(mStartPoint.x() + absXSize, mStartPoint.y());
				poly << QPoint(mStartPoint.x(), mStartPoint.y() + absYSize);
				poly << QPoint(mStartPoint.x(), mStartPoint.y());
				poly << QPoint(mStartPoint.x() - absXSize, mStartPoint.y());
			}


			painter.drawPolygon(poly);
		}
		imageArea.setEdited(true);
		//    int rad(DataSingleton::Instance()->getPenSize() + round(sqrt((mStartPoint.x() - mEndPoint.x()) *
		//                                                                 (mStartPoint.x() - mEndPoint.x()) +
		//                                                                 (mStartPoint.y() - mEndPoint.y()) *
		//                                                                 (mStartPoint.y() - mEndPoint.y()))));
		//    mPImageArea->update(QRect(mStartPoint, mEndPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
		painter.end();
		imageArea.update();

    }
