#include <cctype>
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
#include <bungeegum/glue/imguarded.fwd>

namespace bungeegum {
namespace detail {

    struct widget_inspector {
        widget_inspector() = delete;
        widget_inspector(const widget_inspector&) = delete;
        widget_inspector& operator=(const widget_inspector&) = delete;
        widget_inspector(widget_inspector&&) = delete;
        widget_inspector& operator=(widget_inspector&&) = delete;

        static resolve_command_data& access_resolve(resolve_command& command)
        {
            return command._data;
        }
        static interact_command_data& access_interact(interact_command& command)
        {
            return command._data;
        }
        static draw_command_data& access_draw(draw_command& command)
        {
            return command._data;
        }
    };

    namespace {

        std::string tag(const std::string& name)
        {
            return "###__bungeegum_overlay_inspector_" + name + "__";
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

        std::string clean_fieldname(const std::string& name)
        {
            std::size_t _not_underscore_index = name.find_first_not_of('_');
            std::size_t _length = name.length() - _not_underscore_index;
            std::string _modified = name.substr(_not_underscore_index, _length);
            _modified[0] = static_cast<char>(std::toupper(static_cast<int>(_modified[0])));
            return _modified;
        }

        void draw_json(rapidjson::Document& document, rapidjson::Value& obj, const std::string& name)
        {
            style_guard _cell_padding_guard(ImGuiStyleVar_CellPadding, { 4.f, 0.f });
            if (obj.IsObject()) {
                static ImGuiTableFlags _table_flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBodyUntilResize;
                if (ImGui::BeginTable(tag(name + "json_widget_table_").c_str(), 2, _table_flags)) {
                    ImGui::TableSetupColumn(tag("names_column").c_str(), ImGuiTableColumnFlags_WidthStretch, 0.4f);
                    ImGui::TableSetupColumn(tag("values_column").c_str(), ImGuiTableColumnFlags_WidthStretch, 0.6f);
                    std::vector<std::string> _objects = {};
                    rapidjson::SizeType _count = obj.MemberCount();
                    if (obj.HasMember("BUNGEEGUM_OBJECT_REFERENCE")) {
                        _count--;
                    }

                    for (rapidjson::Value::MemberIterator _iterator = obj.MemberBegin(); _iterator != obj.MemberEnd(); ++_iterator) {
                        if (_iterator->value.IsObject()) {
                            _count--;
                        }
                    }

                    std::size_t _index = 0;
                    for (rapidjson::Value::MemberIterator _iterator = obj.MemberBegin(); _iterator != obj.MemberEnd(); ++_iterator) {
                        std::string _name = _iterator->name.GetString();
                        if (_name != "BUNGEEGUM_OBJECT_REFERENCE") {
                            _name = clean_fieldname(_name);
                            style_guard _no_rounding_guard;
                            if ((_index > 0) && (_index < _count - 1)) {
                                _no_rounding_guard.set(ImGuiStyleVar_FrameRounding, 0.f);
                            }
                            ImGui::TableNextRow();
                            if (_iterator->value.IsInt()) {
                                ImGui::TableSetColumnIndex(0);
                                ImGui::Text(_name.c_str());
                                ImGui::TableSetColumnIndex(1);
                                int1 _int_value = _iterator->value.GetInt();
                                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
                                edit_field<int1>(tag(_name), _int_value);
                                _iterator->value.SetInt(_int_value);

                            } else if (_iterator->value.IsUint()) {
                                ImGui::TableSetColumnIndex(0);
                                ImGui::Text(_name.c_str());
                                ImGui::TableSetColumnIndex(1);
                                uint1 _uint_value = _iterator->value.GetUint();
                                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
                                edit_field<uint1>(tag(_name), _uint_value);
                                _iterator->value.SetUint(_uint_value);

                            } else if (_iterator->value.IsInt64()) {
                                ImGui::TableSetColumnIndex(0);
                                ImGui::Text(_name.c_str());
                                ImGui::TableSetColumnIndex(1);
                                std::int64_t _int64_value = _iterator->value.GetInt64();
                                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
                                edit_field<std::int64_t>(tag(_name), _int64_value);
                                _iterator->value.SetInt64(_int64_value);

                            } else if (_iterator->value.IsUint64()) {
                                ImGui::TableSetColumnIndex(0);
                                ImGui::Text(_name.c_str());
                                ImGui::TableSetColumnIndex(1);
                                std::uint64_t _uint64_value = _iterator->value.GetUint64();
                                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
                                edit_field<std::uint64_t>(tag(_name), _uint64_value);
                                _iterator->value.SetUint64(_uint64_value);

                            } else if (_iterator->value.IsFloat()) {
                                ImGui::TableSetColumnIndex(0);
                                ImGui::Text(_name.c_str());
                                ImGui::TableSetColumnIndex(1);
                                float1 _float_value = _iterator->value.GetFloat();
                                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
                                edit_field<float1>(tag(_name), _float_value);
                                _iterator->value.SetFloat(_float_value);

                            } else if (_iterator->value.IsBool()) {
                                ImGui::TableSetColumnIndex(0);
                                ImGui::Text(_name.c_str());
                                ImGui::TableSetColumnIndex(1);
                                bool1 _bool_value = _iterator->value.GetBool();
                                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
                                edit_field<bool1>(tag(_name), _bool_value);
                                _iterator->value.SetBool(_bool_value);

                            } else if (_iterator->value.IsString()) {
                                ImGui::TableSetColumnIndex(0);
                                ImGui::Text(_name.c_str());
                                ImGui::TableSetColumnIndex(1);
                                std::string _string_value = _iterator->value.GetString();
                                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
                                edit_field<std::string>(tag(_name), _string_value);
                                rapidjson::SizeType _length = static_cast<rapidjson::SizeType>(_string_value.length());
                                _iterator->value.SetString(_string_value.c_str(), _length, document.GetAllocator());

                                // } else if (_iterator->value.IsArray()) { //if array

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

                            } else if (_iterator->value.IsObject()) {
                                _objects.push_back(std::string(_iterator->name.GetString()));
                            }
                            _index++;
                        }
                    }
                    ImGui::EndTable();
                    for (const std::string& _object : _objects) {
                        rapidjson::Value& objValue = obj[_object.c_str()]; //make object value
                        static ImGuiTreeNodeFlags _node_flags = ImGuiTreeNodeFlags_FramePadding;
                        if (ImGui::TreeNodeEx((clean_fieldname(_object) + tag("object_tree_node_" + _object)).c_str(), _node_flags)) {
                            draw_json(document, objValue, _object);
                            ImGui::TreePop();
                        }
                    }
                }
            }
        }

        void draw_inspected_widget_memory_tab(widget_update_data& update_data)
        {
            std::string _title = "memory" + tag("widget_memory_tab");

            if (ImGui::BeginTabItem(_title.c_str())) {

                static ImGuiTableFlags _table_flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBodyUntilResize;
                if (ImGui::BeginTable(tag("info_widget_memory_table").c_str(), 2, _table_flags)) {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("sizeof");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text(std::to_string(update_data.true_sizeof()).c_str());
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("ptr");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text(int_to_hex(update_data.true_ptr()).c_str());
                    ImGui::EndTable();
                }
                ImGui::Spacing();

                if (ImGui::BeginTabBar(tag("widget_memory_tabs_bar").c_str())) {
                    if (ImGui::BeginTabItem(("raw" + tag("widget_memory_raw_tab")).c_str())) {
                        static MemoryEditor _memory_editor;
                        void* _void_ptr = reinterpret_cast<void*>(update_data.true_ptr());
                        std::size_t _raw_size = update_data.true_sizeof();
                        std::size_t _display_address = static_cast<std::size_t>(update_data.true_ptr());
                        _memory_editor.DrawContents(_void_ptr, _raw_size, _display_address);
                        ImGui::EndTabItem();
                    }
                    backend_manager& _backend_manager = global().backend;
                    std::optional<std::string> _serialized = _backend_manager.inspect_reloadable_widget(update_data);
                    static ImGuiTreeNodeFlags _node_flags = ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_CollapsingHeader;
                    if (_serialized.has_value()) {

                        if (ImGui::BeginTabItem(("serialized" + tag("widget_memory_serialized_tab")).c_str())) {
                            ImGui::Spacing();
                            rapidjson::Document _document;
                            _document.Parse(_serialized.value().c_str());
                            rapidjson::Value& _root_value = _document.GetObject()["value0"];
                            draw_json(_document, _root_value, "_root");
                            rapidjson::StringBuffer _json_strbuf;
                            _json_strbuf.Clear();
                            rapidjson::Writer<rapidjson::StringBuffer> _json_writer(_json_strbuf);
                            _document.Accept(_json_writer);
                            std::string _modified = _json_strbuf.GetString();
                            _backend_manager.update_reloadable_widget(update_data, _modified);
                            ImGui::EndTabItem();
                        }
                    }
                }
                ImGui::EndTabBar();

                ImGui::EndTabItem();
            }
        }

        void draw_inspected_widget_resolve_tab(widget_update_data& update_data)
        {
            std::string _title = "resolve" + tag("widget_resolve_tab");
            if (ImGui::BeginTabItem(_title.c_str())) {
                const resolve_command_data& _data = widget_inspector::access_resolve(update_data.resolver_command);
                static ImGuiTableFlags _table_flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBodyUntilResize;

                // _table_size.y = 2.f * ImGui::GetFrameHeight();
                if (ImGui::BeginTable(tag("constraint_widget_resolve_table").c_str(), 2, _table_flags)) {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("constrained min size");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text(to_string(_data.constraint.min_size).c_str());
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("constrained max size");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text(to_string(_data.constraint.max_size).c_str());
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("resolved position");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text(to_string(_data.accumulated_position).c_str());
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("resolved size");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text(to_string(_data.resolved_size).c_str());
                    ImGui::EndTable();
                }
                ImGui::EndTabItem();
            }
        }

        void draw_inspected_widget_interact_tab(const widget_update_data& update_data)
        {
            if (update_data.interactor_command.has_value()) {
                std::string _title = "interact" + tag("widget_interact_tab");
                if (ImGui::BeginTabItem(_title.c_str())) {

                    ImGui::EndTabItem();
                }
            }
        }

        void draw_inspected_widget_draw_tab(widget_update_data& update_data)
        {
            if (update_data.drawer_command.has_value()) {
                std::string _title = "draw" + tag("widget_draw_tab");
                if (ImGui::BeginTabItem(_title.c_str())) {
                    detail::draw_command_data& _data = widget_inspector::access_draw(update_data.drawer_command.value());

                    // GO ALGORITHM
                    //
                    std::size_t _args_count = 0;
                    for (const std::pair<std::string, std::vector<std::string>>& _info : _data.commands_infos) {
                        _args_count = std::max(_args_count, _info.second.size());
                    }
                    //
                    //

                    static ImGuiTableFlags _table_flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBodyUntilResize;
                    if (ImGui::BeginTable(tag("widget_draw_table").c_str(), static_cast<int>(_args_count), _table_flags)) {
                        for (const std::pair<std::string, std::vector<std::string>>& _info : _data.commands_infos) {
                            ImGui::TableNextRow();
                            ImGui::TableSetColumnIndex(0);
                            ImGui::Text(_info.first.c_str());
                            std::size_t _this_args_count = _info.second.size();
                            for (std::size_t _k = 0; _k < _this_args_count - 1; _k++) {
                                ImGui::TableSetColumnIndex(1 + static_cast<int>(_k));
                                ImGui::Text(_info.second[_k].c_str());
                            }
                            for (std::size_t _k = _this_args_count - 1; _k < _args_count - 1; _k++) {
                                ImGui::TableSetColumnIndex(1 + static_cast<int>(_k));
                                ImGui::Text("");
                            }
                        }
                        ImGui::EndTable();
                    }
                    ImGui::EndTabItem();
                }
            }
        }

        bool draw_inspected_widget(const std::uintptr_t raw_widget)
        {
            widgets_manager& _widgets_manager = global().widgets;
            if (_widgets_manager.contains(raw_widget)) {
                widget_update_data& _update_data = _widgets_manager[raw_widget];
                if (ImGui::BeginTabBar(tag("widget_tabs").c_str())) {
                    draw_inspected_widget_memory_tab(_update_data);
                    draw_inspected_widget_resolve_tab(_update_data);
                    draw_inspected_widget_interact_tab(_update_data);
                    draw_inspected_widget_draw_tab(_update_data);
                }
                ImGui::EndTabBar();
                return true;
            }
            return false;
        }
    }

    void draw_inspector_overlay()
    {
        static std::optional<std::string> _opt_title = {};
        ImGui::SetNextWindowSize({ 445, 500 }, ImGuiCond_Once);
        std::string _title = "inspector" + (_opt_title.has_value() ? (" - " + _opt_title.value()) : "") + tag("window_title");
        if (ImGui::Begin(_title.c_str(), NULL, ImGuiWindowFlags_NoCollapse)) {
            backend_manager& _manager = global().backend;
            if (!_manager.inspector_selected.has_value()) {
                ImGui::TextWrapped("Nothing selected. Please select a widget or a running animation in the hierarchy window to display additional content here.");
            } else {
                std::uintptr_t _raw_object = _manager.inspector_selected.value();
                if (!draw_inspected_widget(_raw_object)) {
                    _opt_title = std::nullopt;
                } else {
                    widgets_manager& _widgets_manager = global().widgets;
                    widget_update_data& _update_data = _widgets_manager[_raw_object];
                    _opt_title = _update_data.clean_typename;
                }
            }
        }
        ImGui::End();
    }
}
}