#include "pch.h"

void Object::Render()
{
    auto line = Direct3D::GetInstance()->pLine;
    line->SetAntialias(true);
    line->SetWidth(1.f);

    if ((collisionMode & COLLISION_BOX) == COLLISION_BOX)
    {
        for (auto iter : collisionMasks)
        {
            D3DXVECTOR3 square[5] = {
                D3DXVECTOR3(iter.left + position.x, iter.top + position.y, 0),
                D3DXVECTOR3(iter.right + position.x, iter.top + position.y, 0),
                D3DXVECTOR3(iter.right + position.x, iter.bottom + position.y, 0),
                D3DXVECTOR3(iter.left + position.x, iter.bottom + position.y, 0),
                D3DXVECTOR3(iter.left + position.x, iter.top + position.y, 0)
            };

            D3DXMATRIX lineMat = Direct3D::GetInstance()->viewMat * Direct3D::GetInstance()->projMat;
            line->Begin();
            line->DrawTransform(square, 5, &lineMat, 0xffff0000);
            line->End();
        }
    }
    if ((collisionMode & COLLISION_LINE) == COLLISION_LINE)
    {
        for (auto iter : lineMasks)
        {
            D3DXVECTOR3 vec[2] = {
                D3DXVECTOR3(iter.vP1.x + position.x, iter.vP1.y + position.y, 0),
                D3DXVECTOR3(iter.vP2.x + position.x, iter.vP2.y + position.y, 0)
            };

            D3DXMATRIX lineMat = Direct3D::GetInstance()->viewMat * Direct3D::GetInstance()->projMat;
            line->Begin();
            line->DrawTransform(vec, 2, &lineMat, 0xffff0000);
            line->End();
        }
    }
}

void Object::SetMask(Sprite& sprite)
{
    collisionMasks.clear();

    D3DXVECTOR2 imageSize = D3DXVECTOR2(sprite.GetTextureWidth(), sprite.GetTextureHeight());

    FRECT rc;
    rc.right += imageSize.x;
    rc.top += imageSize.y;

    D3DXVECTOR2 centerFix = D3DXVECTOR2(sprite.center.x * -imageSize.x, sprite.center.y * -imageSize.y);

    rc = rc + centerFix;

    collisionMasks.push_back(rc);

    collisionMode |= COLLISION_BOX;
}

void Object::SetMask(FRECT rc)
{
    collisionMasks.push_back(rc);

    collisionMode |= COLLISION_BOX;
}

void Object::SetLine(CLine line)
{
    lineMasks.push_back(line);

    collisionMode |= COLLISION_LINE;
}
