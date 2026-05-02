#include "pch.h"

void Scene::AddObject(Object* obj)
{
    addObjects.push_back(obj);
}

void Scene::Update(float deltaTime)
{
    vector<Object*> addObjs = addObjects;
    addObjects.clear();
    for (auto iter : addObjs)
        iter->Init();
    objects.insert(objects.end(), addObjs.begin(), addObjs.end());

    sort(objects.begin(), objects.end(), [](Object* p1, Object* p2) {
        return p1->layer < p2->layer;
        });

    for (auto iter : objects)
    {
        iter->Update(deltaTime);
    }
}

void Scene::CollisionUpdate(float deltaTime)
{
    for (auto iter = objects.begin(); iter != objects.end(); ++iter)
    {
        if ((*iter)->isRemove)
            continue;
        if ((*iter)->collisionLayer == 0)
            continue;
        for (auto iter2 = iter + 1; iter2 != objects.end(); ++iter2)
        {
            if ((*iter2)->isRemove)
                continue;
            if ((*iter2)->collisionLayer == 0)
                continue;
            if ((*iter2)->collisionLayer == (*iter)->collisionLayer)
                continue;

            if (((*iter)->collisionMode & COLLISION_BOX) == COLLISION_BOX &&
                ((*iter2)->collisionMode & COLLISION_BOX) == COLLISION_BOX)
            {
                for (auto rc = (*iter)->collisionMasks.begin(); rc != (*iter)->collisionMasks.end(); ++rc)
                {
                    for (auto rc2 = (*iter2)->collisionMasks.begin(); rc2 != (*iter2)->collisionMasks.end(); ++rc2)
                    {
                        FRECT p1, p2;
                        p1 = (*rc) + (*iter)->position;
                        p2 = (*rc2) + (*iter2)->position;
                        if (p1.IsCollision(p2))
                        {
                            (*iter)->OnCollision((*iter2), deltaTime);
                            (*iter2)->OnCollision((*iter), deltaTime);
                            rc = (*iter)->collisionMasks.end() - 1;
                            break;
                        }
                    }
                }
            }

            if (((*iter)->collisionMode & COLLISION_LINE) == COLLISION_LINE &&
                ((*iter2)->collisionMode & COLLISION_LINE) == COLLISION_LINE)
            {
                for (auto line = (*iter)->lineMasks.begin(); line != (*iter)->lineMasks.end(); ++line)
                {
                    for (auto line2 = (*iter2)->lineMasks.begin(); line2 != (*iter2)->lineMasks.end(); ++line2)
                    {
                        CLine p1, p2;
                        p1 = (*line) + (*iter)->position;
                        p2 = (*line2) + (*iter2)->position;
                        if (p1.IsCollision(p2))
                        {
                            (*iter)->OnCollision((*iter2), deltaTime);
                            (*iter2)->OnCollision((*iter), deltaTime);
                            line = (*iter)->lineMasks.end() - 1;
                            break;
                        }
                    }
                }
            }
        }
    }
}

void Scene::LateUpdate(float deltaTime)
{
    for (auto iter = objects.begin(); iter != objects.end();)
    {
        (*iter)->LateUpdate(deltaTime);

        if ((*iter)->isRemove)
        {
            (*iter)->Term();
            delete (*iter);
            iter = objects.erase(iter);
        }
        else
            ++iter;
    }
}

void Scene::Render()
{
    for (auto iter : objects)
        iter->Render();
}

void Scene::ClearScene()
{
    for (auto iter : objects)
        delete iter;
    objects.clear();
    for (auto iter : addObjects)
        delete iter;
    addObjects.clear();
}
