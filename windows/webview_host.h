#pragma once

#include <WebView2.h>
#include <WebView2EnvironmentOptions.h>
#include <wil/com.h>
#include <winrt/Windows.UI.Composition.h>

#include <functional>

#include "webview.h"

using namespace Microsoft::WRL;

struct WebviewCreationError {
  HRESULT hr;
  std::string message;

  explicit WebviewCreationError(HRESULT hr, std::string message)
      : hr(hr), message(message) {}

  static std::unique_ptr<WebviewCreationError> create(
      HRESULT hr, const std::string message) {
    return std::make_unique<WebviewCreationError>(hr, message);
  }
};

class WebviewHost {
 public:
  typedef std::function<void(std::unique_ptr<Webview>,
                             std::unique_ptr<WebviewCreationError>)>
      WebviewCreationCallback;
  typedef std::function<void(wil::com_ptr<ICoreWebView2CompositionController>, std::unique_ptr<WebviewCreationError>)>
      CompositionControllerCreationCallback;

  static std::unique_ptr<WebviewHost> Create(
      std::optional<std::string> user_data_directory = std::nullopt,
      std::optional<std::string> browser_exe_path = std::nullopt,
      std::optional<std::string> arguments = std::nullopt);

  void CreateWebview(HWND hwnd, bool offscreen_only, bool owns_window,
                     WebviewCreationCallback callback);

  winrt::Windows::UI::Composition::Compositor compositor() const {
    return compositor_;
  }

  winrt::Windows::UI::Composition::Visual CreateSurface() const;

 private:
  winrt::Windows::UI::Composition::Compositor compositor_;
  wil::com_ptr<ICoreWebView2Environment3> webview_env_;

  WebviewHost(wil::com_ptr<ICoreWebView2Environment3> webview_env);
  void CreateWebViewCompositionController(
      HWND hwnd, CompositionControllerCreationCallback cb);
};
