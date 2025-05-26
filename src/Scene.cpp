#include "Scene.h"

// src
#include "IRayTraceable.h"

void Scene::AddObject(const std::shared_ptr<IRayTraceable>& object)
{
    ray_traceable_objects_.emplace_back(object);
}

bool Scene::TraceRay(TraceResult& trace_result, const Ray& ray, float min_distance, float max_distance) const
{
    bool did_ray_hit_anything{ false };

    TraceResult local_trace_result{};
    float closest_hit_distance{ max_distance };

    for (const auto& object : ray_traceable_objects_)
    {
        if (object->TraceRay(local_trace_result, ray, min_distance, closest_hit_distance))
        {
            did_ray_hit_anything = true;

            trace_result = local_trace_result;
            closest_hit_distance = trace_result.distance;
        }
    }

    return did_ray_hit_anything;
}
