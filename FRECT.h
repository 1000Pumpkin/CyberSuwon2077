#pragma once

struct FRECT
{
public:
    float left = 0.f, top = 0.f, right = 0.f, bottom = 0.f;

    FRECT operator +(D3DXVECTOR2 pos)
    {
        FRECT rc;
        rc.left = left + pos.x;
        rc.right = right + pos.x;
        rc.top = top + pos.y;
        rc.bottom = bottom + pos.y;
        return rc;
    }

    bool IsCollision(FRECT rc)
    {
        return rc.left <= right
            && rc.right >= left
            && rc.top >= bottom
            && rc.bottom <= top;
    }

    void ResizeWidth(float percent)
    {
        left *= percent;
        right *= percent;
    }
    void ResizeHeight(float percent)
    {
        top *= percent;
        bottom *= percent;
    }
    void Resize(float percent)
    {
        ResizeWidth(percent);
        ResizeHeight(percent);
    }
};

struct CLine
{
public:
    D3DXVECTOR2 vP1, vP2;

    CLine operator +(D3DXVECTOR2 pos)
    {
        CLine line;
        line.vP1 = vP1 + pos;
        line.vP2 = vP2 + pos;
        return line;
    }

    bool IsCollision(CLine line)
    {
        float x1 = vP1.x, x2 = vP2.x, x3 = line.vP1.x, x4 = line.vP2.x;
        float y1 = vP1.y, y2 = vP2.y, y3 = line.vP1.y, y4 = line.vP2.y;

        float den = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);
        if (den == 0.f)
            return false;

        float a = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / den;
        float b = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / den;

        if (a < 0.f || a >1.f || b < 0.f || b > 1.f)
            return false;

        return true;
    }
};