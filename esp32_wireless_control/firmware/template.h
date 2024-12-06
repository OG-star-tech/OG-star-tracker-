#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <Arduino.h>
#include "strings.h"

class Template {
public:
  Template(const char *tmpl);

  // 使用链式调用的方式替换占位符
  Template &replace(const char *placeholder, const char *value);

  Template &replace(const char *placeholder, StringID strId, Language lang);

  Template &replace(const char *placeholder, int value);

  Template &replace(const char *placeholder, float value, int precision = 2);

  Template &replaceChecked(const char *placeholder, bool checked);

  Template &replaceSelected(const char *placeholder, bool selected);

  // 获取最终的HTML字符串
  String toString() const;

private:
  String html;
};

#endif
