// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
#include "DebugGUI.h"
#include "Sokol3DUtils.h"
#include "icons_font_awesome.h"
#include "imgui.h"

using namespace o2::framework;
using namespace o2::framework;

char buf[10];
float f;

int main(int argc, char** argv)
{
  auto context = initGUI("Foo");
  sokol::init3DContext(context);

  auto callback = []() -> void {
    sokol::render3D();
    ImGui::Begin("Hello");
    ImGui::Text("Hello, world %d", 123);
    ImGui::Button("Save");
    ImGui::InputText("string", buf, IM_ARRAYSIZE(buf));
    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
    ImGui::End();
    ImGui::Begin("World");
    ImGui::Text("Sample text");
    ImGui::End();
  };
  while (pollGUI(context, callback)) {
  }
  disposeGUI();
}
