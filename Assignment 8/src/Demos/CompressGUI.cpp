#include "HuffmanResult.h"
#include "Huffman.h"
#include "GUI/ProblemHandler.h"
#include "GUI/TemporaryComponent.h"
#include "GUI/GColorConsole.h"
#include "GUI/GUIMain.h"
#include "ginteractors.h"
#include "filelib.h"
#include "set.h"
#include <string>
#include <fstream>
using namespace std;

namespace {
    class CompressGUI: public ProblemHandler {
    public:
        CompressGUI(GWindow& window);
        void actionPerformed(GObservable* source) override;

    private:
        Temporary<GColorConsole> mDisplay;
        Temporary<GComboBox>     mFileChooser;
        Temporary<GButton>       mCompressButton;
    };

    /* Returns a list of all files we can compress. This excludes anything that we
     * suspect was autogenerated by Qt.
     */
    Set<string> acceptableFiles() {
        Set<string> result;
        for (string file: listDirectory(".")) {
            /* Blacklist any files without extensions. */
            if (file.find('.') == string::npos) continue;

            /* No object / binary files. */
            if (endsWith(toLowerCase(file), ".exe") ||
                endsWith(toLowerCase(file), ".dll") ||
                endsWith(toLowerCase(file), ".o") ||
                endsWith(toLowerCase(file), ".lib") ||
                endsWith(toLowerCase(file), ".a")) continue;

            /* No moc_ files. */
            if (startsWith(file, "moc_")) continue;

            /* No hidden files. */
            if (file.find('.') == 0) continue;

            /* No StanfordCPPLib files. */
            if (file == "iconstrip.png" ||
                file == "splicon-large.png") continue;

            /* Nothing already compressed. */
            if (endsWith(toLowerCase(file), ".huff")) continue;

            result += file;
        }

        return result;
    }

    CompressGUI::CompressGUI(GWindow& window) {
        mDisplay = make_temporary<GColorConsole>(window, "CENTER");

        GComboBox* choices = new GComboBox();
        for (const string& file: acceptableFiles()) {
            choices->addItem(file);
        }
        choices->setEditable(false);
        mFileChooser = Temporary<GComboBox>(choices, window, "SOUTH");

        mCompressButton = make_temporary<GButton>(window, "SOUTH", "Compress");
    }

    string contentsOf(const string& filename) {
        ifstream input(filename, ios::binary);
        if (!input) {
            error("Could not open file " + filename + " for reading.");
        }

        ostringstream buffer;
        buffer << input.rdbuf();
        return buffer.str();
    }

    void decompressFile(GColorConsole& console, const string& filename) {
        console << "Reading file " << filename << "... " << flush;
        string contents = contentsOf(filename);
        console << "done!" << endl;

        console << "Compressing... " << flush;
        HuffmanResult hf = compress(contents);
        console << "done!" << endl;

        console << "Writing result to disk... " << flush;
        ofstream out(filename + ".huff", ios::binary);
        writeHuffmanFile(hf, out);
        out.close();
        console << "done!" << endl;
        console << endl;

        console << "File was saved as " << getCurrentDirectory() << filename << ".huff" << endl;
        console << endl;

        console << "Statistics: " << endl;
        uint64_t oldSize = contents.size();
        uint64_t newSize = fileSize(filename + ".huff");
        console << "  Original file size: " << oldSize << endl;
        console << "  New file size:      " << newSize << endl;
        console << "  Compression ratio:  " << (100.0 * newSize) / oldSize << "%" << endl;
        console << endl;
    }

    void CompressGUI::actionPerformed(GObservable* source) {
        if (source == mCompressButton) {
            setDemoOptionsEnabled(false);
            mCompressButton->setEnabled(false);

            decompressFile(*mDisplay, mFileChooser->getSelectedItem());

            mCompressButton->setEnabled(true);
            setDemoOptionsEnabled(true);
        }
    }
}

shared_ptr<ProblemHandler> makeCompressGUI(GWindow& window) {
    return make_shared<CompressGUI>(window);
}