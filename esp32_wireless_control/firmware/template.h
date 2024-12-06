#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <Arduino.h>
#include "strings.h"

class Template {
public:
  Template(const char *tmpl);

  // Replace placeholders with chained calls
  Template &replace(const char *placeholder, const char *value);

  Template &replace(const char *placeholder, StringID strId, Language lang);

  Template &replace(const char *placeholder, int value);

  Template &replace(const char *placeholder, float value, int precision = 2);

  Template &replaceChecked(const char *placeholder, bool checked);

  Template &replaceSelected(const char *placeholder, bool selected);

  // Get the final HTML string
  String toString() const;

private:
  String html;
};

#endif
