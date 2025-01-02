#include "draw.h"
#include "utils.h"

#include "gm.h"

std::unordered_map<std::size_t, gm::font> font;

gm::draw draw;

gm::api::Real gm_font(gm::api::String sprite_path, gm::api::String glyph_path) noexcept {
    gm::font font{ sprite_path, glyph_path };
    if (!font) {
        return 0;
    }
    std::size_t font_id{ font.id() };
    ::font.emplace(font_id, std::move(font));
    return font_id;
}

gm::api::Real gm_width(gm::api::String text) noexcept {
    return draw.width(gm::utf8_to_ucs2(text));
}

gm::api::Real gm_height(gm::api::String text) noexcept {
    return draw.height(gm::utf8_to_ucs2(text));
}

gm::api::Real gm_draw(gm::api::Real x, gm::api::Real y, gm::api::String text) noexcept {
    return draw.text(x, y, gm::utf8_to_ucs2(text));
}

gm::api::Real gm_free(gm::api::Real font_id) noexcept {
    auto iter{ font.find(static_cast<std::size_t>(font_id)) };
    if (iter == font.end() || &iter->second == draw.setting().font) {
        return false;
    }
    font.erase(iter);
    return true;
}

gm::api::Real gm_clear() noexcept {
    font.clear();
    return true;
}

gm::api::Real gm_set_font(gm::api::Real font_id) noexcept {
    auto iter{ font.find(static_cast<std::size_t>(font_id)) };
    if (iter == font.end()) {
        return false;
    }
    draw.setting().font = &iter->second;
    return true;
}

gm::api::Real gm_set_color(gm::api::Real color) noexcept {
    return gm_set_color2(color, color);
}

gm::api::Real gm_set_color2(gm::api::Real color_top, gm::api::Real color_bottom) noexcept {
    draw.setting().color_top = static_cast<std::uint32_t>(color_top);
    draw.setting().color_bottom = static_cast<std::uint32_t>(color_bottom);
    return true;
}

gm::api::Real gm_set_alpha(gm::api::Real alpha) noexcept {
    if (alpha < 0 || alpha > 1) {
        return false;
    }
    draw.setting().alpha = alpha;
    return true;
}

gm::api::Real gm_set_halign(gm::api::Real align) noexcept {
    draw.setting().halign = align == 0 ? 0 : align < 0 ? -1 : 1;
    return true;
}

gm::api::Real gm_set_valign(gm::api::Real align) noexcept {
    draw.setting().valign = align == 0 ? 0 : align < 0 ? -1 : 1;
    return true;
}

gm::api::Real gm_set_align(gm::api::Real halign, gm::api::Real valign) noexcept {
    gm_set_halign(halign);
    gm_set_valign(valign);
    return true;
}

gm::api::Real gm_set_max_line_width(gm::api::Real max_width) noexcept {
    if (max_width < 0) {
        return false;
    }
    draw.setting().max_line_width = std::abs(max_width);
    return true;
}

gm::api::Real gm_set_letter_spacing(gm::api::Real spacing) noexcept {
    draw.setting().letter_spacing = spacing;
    return true;
}

gm::api::Real gm_set_word_spacing(gm::api::Real spacing) noexcept {
    draw.setting().word_spacing = spacing;
    return true;
}

gm::api::Real gm_set_line_height(gm::api::Real height) noexcept {
    if (height <= 0) {
        return false;
    }
    draw.setting().line_height = height;
    return true;
}

gm::api::Real gm_set_offset(gm::api::Real x, gm::api::Real y) noexcept {
    draw.setting().offset_x = x;
    draw.setting().offset_y = y;
    return true;
}

gm::api::Real gm_set_scale(gm::api::Real x, gm::api::Real y) noexcept {
    if (x <= 0 || y <= 0) {
        return false;
    }
    draw.setting().scale_x = x;
    draw.setting().scale_y = y;
    return true;
}

gm::api::Real gm_get_font() noexcept {
    return draw.setting().font->id();
}

gm::api::Real gm_get_color_top() noexcept {
    return draw.setting().color_top;
}

gm::api::Real gm_get_color_bottom() noexcept {
    return draw.setting().color_bottom;
}

gm::api::Real gm_get_alpha() noexcept {
    return draw.setting().alpha;
}

gm::api::Real gm_get_halign() noexcept {
    return draw.setting().halign;
}

gm::api::Real gm_get_valign() noexcept {
    return draw.setting().valign;
}

gm::api::Real gm_get_max_line_width() noexcept {
    return draw.setting().max_line_width;
}

gm::api::Real gm_get_letter_spacing() noexcept {
    return draw.setting().letter_spacing;
}

gm::api::Real gm_get_word_spacing() noexcept {
    return draw.setting().word_spacing;
}

gm::api::Real gm_get_line_height() noexcept {
    return draw.setting().line_height;
}

gm::api::Real gm_get_offset_x() noexcept {
    return draw.setting().offset_x;
}

gm::api::Real gm_get_offset_y() noexcept {
    return draw.setting().offset_y;
}

gm::api::Real gm_get_scale_x() noexcept {
    return draw.setting().scale_x;
}

gm::api::Real gm_get_scale_y() noexcept {
    return draw.setting().scale_y;
}
