#pragma once

#include <glm/glm.hpp>

#include <bungeegum/glue/infinity.hpp>

namespace bungeegum {

using float2 = glm::vec2;
using float3 = glm::vec3;
using float4 = glm::vec4;

template <>
constexpr float2 infinity<float2> = { infinity<float>, infinity<float> };

template <>
constexpr float3 infinity<float3> = { infinity<float>, infinity<float>, infinity<float> };

template <>
constexpr float4 infinity<float4> = { infinity<float>, infinity<float>, infinity<float>, infinity<float> };

}
