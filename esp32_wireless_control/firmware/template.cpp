#include "template.h"

Template::Template(const char* tmpl) : html(tmpl) {}

Template& Template::replace(const char* placeholder, const char* value) {
  String ph = String("%") + placeholder + "%";
  html.replace(ph, value);
  return *this;
}

Template& Template::replace(const char* placeholder, StringID strId, Language lang) {
  return replace(placeholder, getString(strId, lang));
}

Template& Template::replace(const char* placeholder, int value) {
  return replace(placeholder, String(value).c_str());
}

Template& Template::replace(const char* placeholder, float value, int precision) {
  return replace(placeholder, String(value, precision).c_str());
}

Template& Template::replaceChecked(const char* placeholder, bool checked) {
  return replace(placeholder, checked ? "checked" : "");
}

Template& Template::replaceSelected(const char* placeholder, bool selected) {
  return replace(placeholder, selected ? "selected" : "");
}

String Template::toString() const {
  return html;
}
