#include "util.hpp"
#include "impy.hpp"


int main(int argc, char* argv[]) {
    // GLFWの初期化
    if (!glfwInit()) {
        return -1;
    }
    GLFWwindow* window = glfwCreateWindow(640, 480, "ImGui Viewer", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // gl3wの初期化
    gl3wInit();

    // ImGuiの初期化
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    // フォントの設定
    ImGuiIO& io = ImGui::GetIO();
    ImFontGlyphRangesBuilder builder;
    ImVector<ImWchar> ranges;
    builder.AddText(
        "―‐∥…‥‘’“”≠≦≧∞∴♂♀′″℃☆★○●◎◇◆□■△▲▽▼※→←↑↓∈∋"
        "⊆⊇⊂⊃∪∩∧∨⇒⇔∀∃∠⊥⌒∂∇≡≒≪≫√∽∝∵∫∬Å‰♯♭♪†‡◯ΑΒΓΔΕΖΗΘΙΚΛΜΝ"
        "ΞΟΠΡΣΤΥΦΧΨΩαβγδεζηθικλμνξοπρστυφχψωАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯаб"
        "вгдеёжзийклмнопрстуфхцчшщъыьэюя─│┌┐┘└├┬┤┴┼━┃┏┓┛┗┣┳┫"
        "┻╋┠┯┨┷┿┝┰┥┸╂\uf85b\uf016\uf114\uf15b\ufc6e\ufc0c\uf81f\uf952");
    builder.AddRanges(io.Fonts->GetGlyphRangesChineseFull());
    builder.BuildRanges(&ranges);
    io.Fonts->AddFontFromFileTTF("font/PlemolJP35ConsoleNF-Light.ttf", 15.f, nullptr,
                                 ranges.Data);


    // Pythonのアプリ一覧を取得する
    std::wstring program_folder_path = L"program";
    std::vector<std::string> files = getFoldersAndFiles(program_folder_path);

    bool script_loaded = false;

    // メインループ
    while (!glfwWindowShouldClose(window)) {
        // バッファのクリア
        glClearColor(0.9375f, 0.9375f, 0.9375f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Imguiの初期化
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // カラーテーマ
        ImGui::StyleColorsLight();


        // ================ ファイル選択画面 ================
        // 各種変数
        static bool file_select_window_is_shown = true;
        static char selected_filename[300] = "";
        // ================ ファイル選択表示ボタン ================
        ImGui::BeginMainMenuBar();
        ImGui::Dummy(ImVec2(-14, 1));
        // if (ImGui::BeginMenu("\uf85b ")) {
        if (ImGui::BeginMenu("\uf85b")) {
            if (ImGui::MenuItem("\uf81f  Pythonのファイルを選択する      ")) {
                file_select_window_is_shown = true;
                
                files.clear();
                files.shrink_to_fit();
                files = getFoldersAndFiles(program_folder_path);
            }
            if (ImGui::MenuItem("\uf952  スクリプトをリロードする      ")) {
                script_loaded = impy::run_script(selected_filename);
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
        // ================ ファイル選択ダイアログ ================
        if (file_select_window_is_shown) {
            ImGui::SetNextWindowPos(ImVec2(320, 240), ImGuiCond_Once,
                                    ImVec2(0.5, 0.5));
            ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_Once);
            // window表示
            ImGui::Begin("ファイル選択", &file_select_window_is_shown, ImGuiWindowFlags_NoTitleBar);
            ImGui::Text("■ 実行するファイルを選択してください");
            ImGui::Separator();
            // ファイル一覧(テーブル)
            if (ImGui::BeginTable("##table_folder", 4, ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_ScrollY, ImVec2(0.0f, -33.0f))) {
                ImGui::TableSetupColumn("アイコン", ImGuiTableColumnFlags_WidthFixed, 18.0);
                ImGui::TableSetupColumn("ファイル", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("作成日時", ImGuiTableColumnFlags_WidthFixed, 69.0);
                ImGui::TableSetupColumn("サイズ", ImGuiTableColumnFlags_WidthFixed, 46.0);
                static int id = -1;
                char temp_label[300]; // 使いまわし用ラベル
                for (int row = 0; row < files.size(); row+=3)
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    if(id != row){
                        ImGui::Text(" \uf016 ");
                    }else{
                        ImGui::Text(" \uf15b ");
                    }
                    ImGui::TableSetColumnIndex(1);
                    sprintf(temp_label, files[row].c_str());
                    if(ImGui::Selectable(temp_label, id == row,ImGuiSelectableFlags_SpanAllColumns)){
                        id = row;
                        sprintf(selected_filename, temp_label);
                    }
                    ImGui::TableSetColumnIndex(2);
                    sprintf(temp_label, files[row+1].c_str());
                    ImGui::Text(temp_label);

                    ImGui::TableSetColumnIndex(3);
                    sprintf(temp_label, files[row+2].c_str());
                    ImGui::Text(temp_label);
                }
                ImGui::EndTable();
            }
            ImGui::Separator();
            
            // ファイル名表示欄
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
            ImGui::PushStyleColor(ImGuiCol_Button,
                                  ImVec4{0.879f, 0.879f, 0.879f, 1.0f});
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                                  ImVec4{0.664f, 0.797f, 0.953f, 1.0f});
            ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                                  ImVec4{0.564f, 0.697f, 0.953f, 1.0f});
            ImGui::PushStyleColor(ImGuiCol_FrameBg,
                                  ImVec4{0.900f, 0.900f, 0.900f, 1.0f});
            ImGui::Dummy(ImVec2(0,0.1f));
            ImGui::SetNextItemWidth(-43.0f);
            ImGui::InputText("##selected_filename",selected_filename, sizeof(selected_filename), ImGuiInputTextFlags_ReadOnly);
            ImGui::SameLine();

            if(ImGui::Button("実行") && selected_filename[0] != '\0'){
                // ここで読み込む
                script_loaded = impy::run_script(selected_filename);
                //std::cout << selected_filename << std::endl;
                file_select_window_is_shown = false;
            }

            ImGui::PopStyleColor(4);
            ImGui::PopStyleVar(1);
            
            ImGui::End();
        }
        // ================================================

        if(script_loaded){
            impy::draw();
        }


        // Imguiの画面更新
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // ダブルバッファのスワップ
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ImGuiの終了処理
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // GLFWの終了処理
    glfwTerminate();

    return 0;
}
