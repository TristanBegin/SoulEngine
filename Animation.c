//#include "Animation.h"
//
//void Animate(SPRITE * pSprite)
//{
//	AEGfxTextureSet(pSprite->pTexture, pSprite->Offset.x , pSprite->Offset.y);
//	
//	if ((pSprite->Offset.y + 1 / pSprite->RowCol.y >= 1) 
//		&& (pSprite->Offset.x + 1 / pSprite->RowCol.x >= 1))
//	{
//		pSprite->Offset.x = 0;
//		pSprite->Offset.y = 0;
//	}
//	else if(pSprite->Offset.y + 1 / pSprite->RowCol.y < 1)
//	{
//		pSprite->Offset.y += 1 / pSprite->RowCol.y;
//	}
//	else //if (pSprite->Offset.y + 1 / pSprite->RowCol.y >= 1)
//	{
//		pSprite->Offset.y = 0;
//		pSprite->Offset.x += 1 / pSprite->RowCol.x;
//	}
//}
