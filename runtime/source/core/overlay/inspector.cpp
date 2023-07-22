#include <string>

#include <imgui.h>
#include <imgui_memory_editor.h>
#include <implot.h>
#include <iostream>

#include <bungeegum/core/animation.hpp>
#include <bungeegum/core/global.fwd>
#include <bungeegum/core/overlay.fwd>
#include <bungeegum/core/widget.hpp>
#include <bungeegum/glue/imedit.hpp>

namespace bungeegum {
namespace detail {

    namespace {

        std::string tag(const std::string& name)
        {
            return "###__bungeegum_overlay_inspector_" + name + "__";
        }

        std::string size_to_string(const float2 size)
        {
            return (std::to_string(size.x) + ", " + std::to_string(size.y));
        }

        template <typename T>
        std::string int_to_hex(T i)
        {
            std::stringstream stream;
            stream << "0x"
                   << std::setfill('0') << std::setw(sizeof(T) * 2)
                   << std::hex << i;
            return stream.str();
        }

        void draw_json(const rapidjson::Value& obj, size_t indent = 0)
        {
            if (obj.IsObject()) {
                for (rapidjson::Value::ConstMemberIterator _iterator = obj.MemberBegin(); _iterator != obj.MemberEnd(); ++_iterator) { //iterate through object
                    bool _found = false;
                    std::string _value;
                    for (size_t _k = 0; _k != indent; ++_k) {
                        _value += " ";
                    }
                    std::string _name(_iterator->name.GetString());
                    if (_name != "BUNGEEGUM_OBJECT_REFERENCE") {
                        _value += _name + std::string(": ");
                        if (_iterator->value.IsUint()) {
                            _value += std::to_string(_iterator->value.GetUint());
                            _found = true;
                        } else if (_iterator->value.IsString()) {
                            _value += _iterator->value.GetString();
                            _found = true;
                        } else if (_iterator->value.IsInt()) {
                            _value += std::to_string(_iterator->value.GetInt());
                            _found = true;
                        } else if (_iterator->value.IsUint()) {
                            _value += std::to_string(_iterator->value.GetUint());
                            _found = true;
                        } else if (_iterator->value.IsInt64()) {
                            _value += std::to_string(_iterator->value.GetInt64());
                            _found = true;
                        } else if (_iterator->value.IsUint64()) {
                            _value += std::to_string(_iterator->value.GetUint64());
                            _found = true;
                        } else if (_iterator->value.IsFloat()) {
                            _value += std::to_string(_iterator->value.GetFloat());
                            _found = true;
                            float _float_value = _iterator->value.GetFloat();
                            edit_field(_name, _float_value);
                            // set
                        } else if (_iterator->value.IsBool()) {
                            _value += std::to_string(_iterator->value.GetBool());
                            _found = true;
                        } else if (_iterator->value.IsArray()) {
                            _value += std::to_string(_iterator->value.GetBool());
                            _found = true;

                            // array of obj etc
                        }

                        if (_found) {
                            ImGui::Text(_value.c_str());
                        } else if (_iterator->value.IsObject()) {
                            const rapidjson::Value& objName = obj[_iterator->name.GetString()]; //make object value
                                // TREE PUSH
                            draw_json(objName, indent + 1);
                            // TREE POP
                        }
                    }
                }

                // else if (_iterator->value.IsArray()) { //if array

                //     for (rapidjson::SizeType i = 0; i < _iterator->value.Size(); i++) {
                //         if (_iterator->value[i].IsNumber()) //if array value integer
                //             std::cout << _iterator->value[i].GetInt();

                //         else if (_iterator->value[i].IsString()) //if array value string
                //             std::cout << _iterator->value[i].GetString();

                //         else if (_iterator->value[i].IsBool()) //if array value bool
                //             std::cout << _iterator->value[i].GetBool();

                //         else if (_iterator->value[i].IsObject()) { //if array value object
                //             std::cout << "\n  ";
                //             const rapidjson::Value& m = _iterator->value[i];
                //             for (auto& v : m.GetObject()) { //iterate through array object
                //                 if (m[v.name.GetString()].IsString()) //if array object value is string
                //                     std::cout << v.name.GetString() << ": " << m[v.name.GetString()].GetString();
                //                 else //if array object value is integer
                //                     std::cout << v.name.GetString() << ": " << m[v.name.GetString()].GetInt();

                //                 std::cout << "\t"; //indent
                //             }
                //         }
                //         std::cout << "\t"; //indent
                //     }
                // }
            }
        }

        bool draw_inspected_widget(const std::uintptr_t raw_widget)
        {
            widgets_manager& _widgets_manager = global().widgets;
            backend_manager& _backend_manager = global().backend;
            if (_widgets_manager.contains(raw_widget)) {
                widget_update_data& _update_data = _widgets_manager[raw_widget];

                std::optional<std::string> _serialized = _backend_manager.inspect_reloadable_widget(_update_data);

                if (_serialized.has_value()) {
                    rapidjson::Document _document;
                    _document.Parse(_serialized.value().c_str());
                    const rapidjson::Value& _root_value = _document.GetObject()["value0"];
                    draw_json(_root_value);
                }

                ImGui::Text(("sizeof =" + std::to_string(_update_data.true_sizeof())).c_str());
                ImGui::Text(("raw =" + int_to_hex(_update_data.true_ptr())).c_str());

                static MemoryEditor mem_edit_2;
                mem_edit_2.DrawContents((void*)_update_data.true_ptr(), _update_data.true_sizeof(), _update_data.true_ptr());

                ImGui::Text("Constraints");
                ImGui::Text(("min_size = " + size_to_string(_update_data.resolver_command.min_size())).c_str());
                ImGui::Text(("max_size = " + size_to_string(_update_data.resolver_command.max_size())).c_str());

                if (!_update_data.children.empty())
                    ImGui::Text((std::to_string(_update_data.children.size()) + " children :").c_str());
                return true;
            }
            return false;
        }

        bool draw_inspected_animation(const std::uintptr_t raw_animation)
        {
            animations_manager& _manager = global().animations;
            if (_manager.contains(raw_animation)) {
                const animation_update_data& _update_data = _manager[raw_animation];
                static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_NoHighlight | ImPlotAxisFlags_NoTickMarks;
                ImPlot::PushStyleColor(ImPlotCol_FrameBg, { 1.f, 1.f, 0.f, 0.f });
                if (ImPlot::BeginPlot(tag("animation_graph").c_str(), { ImGui::GetContentRegionAvail().x, 90 }, flags)) {
                    ImPlot::SetupAxes(NULL, NULL, flags, flags);
                    ImPlot::SetupAxisLimits(ImAxis_X1, 0, 1, ImGuiCond_Always);
                    ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 1);
                    ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
                    auto _samples = _update_data.overlay_samples;
                    auto _point = _update_data.overlay_position;
                    ImPlot::PlotLine("", _samples.data(), &(_samples[1]), 100, 0, 0, 2 * sizeof(float));
                    ImPlot::PlotScatter("", _point.data(), _point.data() + 1, 1, 0, 0, 2 * sizeof(float));
                    ImPlot::EndPlot();
                }
                ImPlot::PopStyleColor();
                return true;
            }
            return false;
        }
    }

    void draw_inspector_overlay()
    {
        ImGui::SetNextWindowSize({ 300, 450 }, ImGuiCond_Once);
        if (ImGui::Begin(("inspector" + tag("window_title")).c_str(), NULL, ImGuiWindowFlags_NoCollapse)) {
            backend_manager& _manager = global().backend;
            if (!_manager.inspector_selected.has_value()) {
                ImGui::Text("No widget selected.");
            } else {
                std::uintptr_t _raw_object = _manager.inspector_selected.value();
                if (!draw_inspected_widget(_raw_object)) {
                    draw_inspected_animation(_raw_object);
                }
            }
        }
        ImGui::End();
    }
}
}