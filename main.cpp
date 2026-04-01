/*
    Name:       Laurel Markos
    Project:    Wire Minnow

    What is this?

        This is the main driver function for the application. Though currently this is the entire application, this
        will later become the driver. This main file will serve as the main controller for the program's flow, while
        the packet capturing functionality will be within another file. This driver is primarily focused on the
        initialization of windows, panels, and other `ncurses` functionality.

*/

#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <algorithm>
#include <math.h>
#include <ncurses.h>
#include <panel.h>
#include "keys.hpp"
#include "enums.hpp"
// #include <MacAddress.h>

void recording_driver(WINDOW* win, PANEL* pan);


int main(int argc, char* argv[]) {

    using namespace std;

    int row, col;
    int trow, tcol, prow, pcol, x, y;
    const string title = "Wire Minnow";
    const string helpmsg = "For Help, press [CTRL-H]";

    initscr();
    
    // Makes Cursor Invisible

    curs_set(0);
    
    cbreak();                   // I am not fully sure what this is useful for, as our commands ran fine before.

    // Help Window/Panel Setup
    getmaxyx(stdscr,row, col);
    
    prow = 20;
    pcol = 40;
    x = 0.5 * row - (0.5*prow);
    y = 0.5 * col - (0.5*pcol);

    WINDOW* helpwin = newwin(prow, pcol, x, y);
    PANEL* helppan = new_panel(helpwin);

    box(helpwin, 0,0);
    wattron(helpwin, A_BOLD);
    mvwprintw(helpwin, 0, 2, "| Command Help |");
    wattroff(helpwin, A_BOLD);


    vector<pair<string, string>> help_commands = {
        {"[CTRL-H]","\t Show Help Menu"},
        {"[CTRL-X]","\t Exit Wire Minnow"},
        {"[CTRL-P]","\t Play/Pause Recording"},
        {"[CTRL-F]","\t Filter by Property"},
        {"[CTRL-S]","\t Search for Packet"}
    };

    // Display all of the help commands in the helpwin window.
    // Write to the help window before display to reduce repetition layer on.
    // This may be altered to a Menu in the future ***

    for (unsigned long hcol = 2; hcol <= help_commands.size() * 2; hcol += 2) {
        wattron(helpwin, A_BOLD);
        mvwprintw(helpwin, hcol, 2, "%s", help_commands.at((hcol / 2) - 1).first.c_str());
        wattroff(helpwin, A_BOLD);
        mvwprintw(helpwin, hcol, 11, "%s", help_commands.at((hcol / 2) - 1).second.c_str());
    }
    
    // Recording Window Setup

    WINDOW* recwin = newwin(row, col, 0, 0);
    PANEL* recpan = new_panel(recwin);

    box(recwin, 0, 0);
    wattron(recwin, A_BOLD);
    mvwprintw(recwin, 0, 2, " Recording Window ");
    wattroff(recwin, A_BOLD);

    // Title msg

    trow = 0.3*row;
    tcol = (0.5 * col) - (0.5 * title.length());
    attron(A_BOLD);
    mvprintw(trow, tcol, "%s", title.c_str());
    attroff(A_BOLD);

    // Help msg

    trow = 0.7 * row;
    tcol = (0.5 * col) - (0.5 * helpmsg.length());
    mvprintw(trow, tcol, "%s", helpmsg.c_str());

    refresh();
    wrefresh(helpwin);
    wrefresh(recwin);
    hide_panel(helppan);
    hide_panel(recpan);

    // Input Section

    noecho();

    while(true) {
        switch(getch()){
            case CTRL_H:
                // Show Popup Window
                // printw("Button Pressed");
                if(panel_hidden(helppan)) {
                    show_panel(helppan);
                } else {
                    hide_panel(helppan);
                }
                update_panels();
                doupdate();
                break;
            case CTRL_P:
                // Begin Recording
                show_panel(recpan);
                recording_driver(recwin, recpan);
                hide_panel(recpan);

                // This should later be changed to 
                update_panels();
                doupdate();
                break;
            case CTRL_X:
                endwin();
                return 0;
                break;
        }
        refresh();

        // Naps for a ms so the computer doesn't explode
    }

    endwin();

    return 1;
}


/*
    Purpose
        Acts as the main driver for the recording process.
    Input
        WINDOW* -- Used for writing to the recwin defined in main
        PANEL*  -- Used for managing te recpan if needed
    Output
        void
*/

void recording_driver(WINDOW* win, PANEL* pan) {

    using namespace std;
    int input;
    int selected = 0;
    vector<pair<string, Sort>> header = {
        {"No.",NA},
        {"Time",NA},
        {"Source",NA},
        {"Destination",NA},
        {"Protocol",NA},
        {"Length", NA},
        {"Info",NA}
    };

        noecho();
        keypad(win, true);
        cbreak();

    while (true) {
        int spacing = 2;
        char sortBy;
        // Used so wgetch isn't blocking when implementing pcap later
        wtimeout(win, 0);

        input = wgetch(win);

        switch(input) {

            case KEY_RIGHT:
                if (selected < static_cast<int>(header.size()-1))
                    selected++;
                break;
            case KEY_LEFT:
                if(selected > 0)
                    selected--;
                break;
            case KEY_ENTR:
                for(int i = 0; i < static_cast<int>(header.size()); i++) {
                    if (i != selected)
                        header.at(i).second = NA;
                }
                header.at(selected).second = static_cast<Sort>((header.at(selected).second + 1) % 3);
                break;
            case CTRL_X:
                return;
        }

        
        for (int i = 0; i < static_cast<int>(header.size()); i++) {
            if (i == selected)
                wattron(win, A_REVERSE);
            wattron(win, A_BOLD);
            mvwprintw(win, 2, spacing, "%s", header.at(i).first.c_str());
            wattroff(win, A_BOLD);
            wattroff(win, A_REVERSE);

            spacing += header.at(i).first.length();

            if (header.at(i).second == LOW) {
                sortBy = '-';
            } else if (header.at(i).second == HIGH) {
                sortBy = '+';
            } else {
                sortBy = ' ';
            }

            mvwprintw(win, 2, spacing, " %c | ", sortBy);

            spacing += 5;
            
        }


        wrefresh(win);
        update_panels();
        doupdate();

        napms(1);
    }

    
    return;
}