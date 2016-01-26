#include "Animation.h"
#include <time.h>
void Animate(SPRITE * pSprite)
{
	
	if (pSprite->TimeSinceLastFrame > 1 / (5 *pSprite->AnimationSpeed))
	{
		pSprite->TimeSinceLastFrame = 0;
		// If you are at the last frame of animation.
		if ((pSprite->Offset.y + 1 / pSprite->RowCol.y >= 1)
			&& (pSprite->Offset.x + 1 / pSprite->RowCol.x >= 1))
		{
			// Go back to the first frame
			pSprite->Offset.x = 0;
			pSprite->Offset.y = 0;
		}
		// If you are not at the end of the x. (so if x added with the frame size is < 1)
		else if (pSprite->Offset.x + 1 / pSprite->RowCol.x < 1)
		{
			// Move the frame once to the right(x).
			pSprite->Offset.x += 1 / pSprite->RowCol.x;
		}
		// If you are at the end of the x. (so if x added with the frame size is >= 1)
		else
		{
			// Move the frame to the first column(x = 0)
			pSprite->Offset.x = 0;
			// Move the frame down(y) once
			pSprite->Offset.y += 1 / pSprite->RowCol.y;
		}
	}
	else
	{
		pSprite->TimeSinceLastFrame += (float)AEFrameRateControllerGetFrameTime();
	}
}
