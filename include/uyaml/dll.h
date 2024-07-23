#pragma once

#ifdef UYAML_USE_STATIC
#define UYAML_API
#else
#ifdef UYAML_EXPORT
#define UYAML_API __declspec(dllexport)
#else
#define UYAML_API __declspec(dllimport)
#endif
#endif
