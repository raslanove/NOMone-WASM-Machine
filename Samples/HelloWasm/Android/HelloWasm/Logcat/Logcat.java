import java.awt.AWTException;
import java.io.*;
import java.util.ArrayList;
import java.awt.Robot;
import java.awt.event.KeyEvent;

/**
 * Created by raslanove on 7/17/20.
 *
 */

public class Logcat {

    private static final String ADB_FILE_PATH = "adb";

    // Customizations,
    private static final Color     V_COLOR = Color.RESET;
    private static final Color     D_COLOR = Color.RESET;
    private static final Color     I_COLOR = Color.RESET;
    private static final Color     W_COLOR = Color.BLUE;
    private static final Color     E_COLOR = Color.RED_BRIGHT;
    private static final Color  HINT_COLOR = Color.MAGENTA_BOLD_BRIGHT;
    private static final Color OTHER_COLOR = Color.GREEN_BOLD_BRIGHT;

    private static final int       DATE_LENGTH =   5;
    private static final int       TIME_LENGTH =  12;
    private static final int PROCESS_ID_LENGTH =   5;
    private static final int  THREAD_ID_LENGTH =   5;
    private static final int  LOG_LEVEL_LENGTH =   1;
    private static final int        TAG_LENGTH =  20;
    private static final int    MESSAGE_LENGTH = 110; //188

    private static final String SEPARATOR = " | ";
    private static final String CONTINUATION = "→";
    private static final String INDENTATION = "  ";

    private static final int PROCESS_IDS_UPDATE_INTERVAL_MILLIS = 1224;

    private static final int HISTORY_LENGTH = 1000;

    // State,
    private static boolean skipProcessIDCheck;
    private static ArrayList<String> processIDs = new ArrayList<String>();

    private static String logLevelToShow="V";  // All.

    private static Process logcatProcess;
    private static boolean appClosed;
    private static boolean stopEverything;

    private static String[] history = new String[HISTORY_LENGTH];
    private static int currentLocationInHistory, historyLength;

    public static void main(final String args[]) {

        clearAndroidStudioConsole();
        System.out.println("besm Allah");

        // Get processes ids of the provided package,
        if (args.length==0) {
            skipProcessIDCheck = true;
        } else {
            skipProcessIDCheck = false;
            getProcessIDs    (args[0]);    // Do it once before we start.
            monitorProcessIDs(args[0]);    // Do it periodically from now on.
        }

        // Start capturing and prettifying logcat,
        if (!monitorLogcat()) {
            stopEverything = true;
            return;
        }

        // Handle user input,
        handleUserInput();
    }

    private static void watch(final Process process, final ProcessListener listener) {

        // Read process standard output and send it to the listener line by line,
        new Thread() {
            public void run() {
                BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(process.getInputStream()));
                String line = "";
                try {
                    do {
                        if (bufferedReader.ready()) {
                            line = bufferedReader.readLine();
                            if (line!=null && !line.isEmpty()) listener.onNewLine(line);
                        } else {
                            Thread.sleep(100);
                        }
                    } while (line!=null && !stopEverything);
                } catch (Exception e) { e.printStackTrace(); }
            }
        }.start();
    }

    private static void monitorProcessIDs(String packageName) {

        // Continuously monitor the process IDs of this package and update when changed,
        new Thread() {
            public void run() {
                do {
                    try { Thread.sleep(PROCESS_IDS_UPDATE_INTERVAL_MILLIS); } catch (InterruptedException e) {}
                    getProcessIDs(packageName);
                } while (!stopEverything);
            }
        }.start();
    }

    private static void getProcessIDs(String packageName) {

        // Get the process IDs associated with this package once,
        ArrayList<String> newProcessIDs = new ArrayList<String>();
        Runtime runtime = Runtime.getRuntime();
        try {
            Process getPIDProcess = runtime.exec(ADB_FILE_PATH + " shell ps");
            watch(getPIDProcess, (line) -> {
                if (line.contains(packageName)) {
                    newProcessIDs.add(removeRedundantSpaces(line).split(" ")[1]);
                }
            });
            getPIDProcess.waitFor();
            Thread.sleep(500);  // Make sure we've already handled all the input from the process.
        } catch (Exception e) { e.printStackTrace(); }

        // Return immediately if program is closed,
        if (stopEverything) return ;

        // Some action upon getting the pid(s),
        boolean shouldRepeatHistory = false;
        if (newProcessIDs.isEmpty()) {

            // Just closed,
            if (!appClosed) {
                appClosed = true;
                prettify("----- App closed -----");
            }
        } else if (appClosed) {

            // Just opened, clear,
            appClosed = false;
            clearAndroidStudioConsole();
            prettify("----- App opened -----");
            shouldRepeatHistory = true;
        } else {

            // Detect changes in processes,
            for (String pid : newProcessIDs) {
                if (!processIDs.contains(pid)) {
                    clearAndroidStudioConsole();
                    prettify("----- Process(es) changed (or app restarted - some logs could have been missed) -----");
                    shouldRepeatHistory = true;
                    break ;
                }
            }
        }

        // Set the new PID(s),
        processIDs = newProcessIDs;
        if (shouldRepeatHistory) repeatHistory();
    }

    private static boolean monitorLogcat() {

        Runtime runtime = Runtime.getRuntime();
        try {
            logcatProcess = runtime.exec(ADB_FILE_PATH + " logcat -v threadtime");
            watch(logcatProcess, (line) -> {

                // Learn history, in case we need to repeat it,
                if (appClosed || processLogcatLine(line)) {
                    history[currentLocationInHistory] = line;
                    currentLocationInHistory = (currentLocationInHistory + 1) % history.length;
                    if (historyLength<history.length) historyLength++;
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }

        return true;
    }

    private static boolean processLogcatLine(String line) {
        try {
            return prettify(line);
        } catch (Exception e) {
            print(line, OTHER_COLOR);
            System.out.println();

            // Debug,
            e.printStackTrace();
            return true;
        }
    }

    // Returns true if line should be kept in history,
    private static synchronized boolean prettify(String line) {

        if (line.startsWith("-")) {
            // It's a "beginning of <something>" line,
            print(line, HINT_COLOR);
            System.out.println();
            return true;
        }

        // Get the individual fields,
        String      date = line.substring(0, line.indexOf(' ')); line = line.substring(line.indexOf(' ')+1); line = line.trim();
        String      time = line.substring(0, line.indexOf(' ')); line = line.substring(line.indexOf(' ')+1); line = line.trim();
        String processID = line.substring(0, line.indexOf(' ')); line = line.substring(line.indexOf(' ')+1); line = line.trim();

        // Break early if possible,
        if (!skipProcessIDCheck && !processIDs.contains(processID.trim())) return false;

        // Continue parsing,
        String  threadID = line.substring(0, line.indexOf(' ')); line = line.substring(line.indexOf(' ')+1); line = line.trim();
        String  logLevel = line.substring(0, line.indexOf(' ')); line = line.substring(line.indexOf(' ')+1); line = line.trim();

        // Break early if possible,
        switch (logLevel) {
            case "V": if (!"V"    .contains(logLevelToShow)) return true; break;
            case "D": if (!"VD"   .contains(logLevelToShow)) return true; break;
            case "I": if (!"VDI"  .contains(logLevelToShow)) return true; break;
            case "W": if (!"VDIW" .contains(logLevelToShow)) return true; break;
            case "E": if (!"VDIWE".contains(logLevelToShow)) return true; break;
        }

        // Continue parsing,
        String       tag = line.substring(0, line.indexOf(':')); line = line.substring(line.indexOf(':')+1);

        // Now let's process the message,
        line = trimString(line); // Trim preserving ANSI escape codes for colors.

        // Some tags have a trailing ":",
        if (line.startsWith(":")) {
            tag += ":";
            line = line.substring(1);
        }

        // Indent lines starting by "at",
        String indentation = "";
        if (line.startsWith("at ")) {
            indentation = "   " + INDENTATION;
            line = " " + INDENTATION + line;
        }

        // Print the prettified log,
        Color color;
        switch (logLevel) {
            case "V": color = V_COLOR; break;
            case "D": color = D_COLOR; break;
            case "I": color = I_COLOR; break;
            case "W": color = W_COLOR; break;
            case "E": color = E_COLOR; break;
            default:
                color = Color.RESET;
        }

        String fields = adjustLength(     date,       DATE_LENGTH) + SEPARATOR +
                        adjustLength(     time,       TIME_LENGTH) + SEPARATOR +
                        adjustLength(processID, PROCESS_ID_LENGTH) + SEPARATOR +
                        adjustLength( threadID,  THREAD_ID_LENGTH) + SEPARATOR +
                        adjustLength( logLevel,  LOG_LEVEL_LENGTH) + SEPARATOR +
                        adjustLength(      tag,        TAG_LENGTH) + SEPARATOR;

        // Split the message onto multiple lines if needed,
        String message = chunkPreservingParentheses(line, MESSAGE_LENGTH, 2);
        print(fields + message, color);
        System.out.println();

        while (line.length() > message.length()) {

            // Debug,
            //print(line, OTHER_COLOR);
            //System.out.println("Line: " + line.length() + "length: " + message.length() + ", cont: " + CONTINUATION.length() + "dent: " + indentation.length());
            //System.out.println();

            // Remove the already printed part.
            line = line.substring(message.length()-CONTINUATION.length());

            // Add a dot to make links work,
            boolean shouldAddDot=false;
            if (line.matches("^[^\\.]*\\(.*:[123456789][1234567890]*\\).*")) shouldAddDot = true;

            // Indent,
            line = (shouldAddDot ? "." : (indentation.isEmpty() ? "" : " ")) + indentation + line;

            // Take another chunk,
            message = chunkPreservingParentheses(line, MESSAGE_LENGTH, 2+indentation.length());

            // Front pad to align this part with the message body,
            String paddedMessage = message;
            for (int i=0; i<fields.length(); i++) paddedMessage = ' ' + paddedMessage;

            // Print,
            System.out.println(paddedMessage);
        }

        return true;  // Keep in local buffer.
    }

    private static String adjustLength(String text, int length) {
        while (text.length() < length) text += ' ';
        if (text.length() > length) {
            text = text.substring(0, length-CONTINUATION.length());
            text += CONTINUATION;
        }
        return text;
    }

    private static String chunkPreservingParentheses(String text, int length, int minChunckLength) {

        if (text.length() <= length) return text;

        // Take a chunk out of the text,
        String chunk = text.substring(0, length-CONTINUATION.length()) + CONTINUATION;

        // Check if a paranthesis was opened and not closed,
        int lastOpenParanthesisIndex = chunk.lastIndexOf('(');
        int lastCloseParanthesisIndex = chunk.lastIndexOf(')');
        if (lastCloseParanthesisIndex <= lastOpenParanthesisIndex) {  // Also works when either is not found.
            if (minChunckLength<1) minChunckLength = 1;
            if (lastOpenParanthesisIndex > minChunckLength+CONTINUATION.length()) { // Avoid endless loops.
                int includeParenthesisSize = (CONTINUATION.length()>0) ? 1 : 0;
                chunk = text.substring(0, lastOpenParanthesisIndex+includeParenthesisSize-CONTINUATION.length()) + CONTINUATION;
            }
        }

        return chunk;
    }

    private static void repeatHistory() {
        int index = currentLocationInHistory-historyLength;
        if (index < 0) index += history.length;
        for (int i=0; i<historyLength; i++) {
            processLogcatLine(history[index]);
            index = (index + 1) % history.length;
        }
    }

    private static void print(String text, Color color) {
        System.out.print(color);
        System.out.print(text);
    }

    private static String trimString(String string) {

        // Because the java provided String.trim() removes the ANSI escape codes
        // ('\033' characters) which are necessary for coloring in ANSI-compatible
        // terminals.
        int textLength = string.length();

        int frontTrimIndex=0;
        for (; frontTrimIndex<textLength; frontTrimIndex++) {
            char currentChar = string.charAt(frontTrimIndex);
            if (currentChar==' ' || currentChar=='\t' || currentChar=='\n' || currentChar=='\r') continue;
            break;
        }

        int backTrimIndex=textLength-1;
        for (; backTrimIndex>-1; backTrimIndex--) {
            char currentChar = string.charAt(backTrimIndex);
            if (currentChar==' ' || currentChar=='\t' || currentChar=='\n' || currentChar=='\r') continue;
            break;
        }

        return string.substring(frontTrimIndex, backTrimIndex+1);
    }

    private static String removeRedundantSpaces(String text) {
        String newText = text.replace("  ", " ");
        while (!text.equals(newText)) {
            text = newText;
            newText = text.replace("  ", " ");
        }
        return text;
    }

    private static void clearAndroidStudioConsole() {

        // Couldn't find a reliable way to clear Intellij terminal scrollback, so we just print
        // a LOT of newlines,
        StringBuilder bunchOfNewLines = new StringBuilder();
        for (int i=0; i<124; i++) bunchOfNewLines.append(System.lineSeparator());
        System.out.print(bunchOfNewLines);

        // Scroll the current line to the top of the window,
        try {
            // If we are on Windows,
            new ProcessBuilder("cmd", "/c", "cls").inheritIO().start().waitFor();
        } catch (Exception e) {

            // We are not on Windows,
            bunchOfNewLines = new StringBuilder();
            for (int i=0; i<124; i++) bunchOfNewLines.append("\b\r");
            System.out.print(bunchOfNewLines);
        }
    }

    private static void handleUserInput() {

        // Line read. Unfortunately, java doesn't provide character by character reading out of the box.
        BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(System.in));
        String input = "";
        do {
            try {
                if (bufferedReader.ready()) {
                    input = input = bufferedReader.readLine().toUpperCase();

                    // Set log level,
                    if (input.equals("V")||input.equals("D")||input.equals("I")||input.equals("W")||input.equals("E")) {

                        if (!logLevelToShow.equals(input)) {
                            logLevelToShow = input;
                            clearAndroidStudioConsole();
                            repeatHistory();
                        }

                        prettify("----- Log level set to " + logLevelToShow + " -----");

                    } else if (input.equals("C")) {

                        // Clear screen and history,
                        clearAndroidStudioConsole();
                        historyLength = 0;
                    }
                } else {
                    Thread.sleep(100);
                }
            } catch (Exception e) { e.printStackTrace(); }

            // Check if the logcat process is still alive,
            if (!logcatProcess.isAlive()) {
                prettify("----- adb logcat process terminated -----");
                stopEverything = true;
            }

        } while (!stopEverything && !input.equals("Q"));

        // Allow all monitoring threads to exit,
        stopEverything = true;
    }

    interface ProcessListener {
        void onNewLine(String line);
    }

    enum Color {

        // Thanks to this answer: https://stackoverflow.com/a/51944613/1942069

        //Color end string, color reset
        RESET("\033[0m"),

        // Regular Colors. Normal color, no bold, background color etc.
        BLACK  ("\033[0;30m"),
        RED    ("\033[0;31m"),
        GREEN  ("\033[0;32m"),
        YELLOW ("\033[0;33m"),
        BLUE   ("\033[0;34m"),
        MAGENTA("\033[0;35m"),
        CYAN   ("\033[0;36m"),
        WHITE  ("\033[0;37m"),

        // Bold
        BLACK_BOLD  ("\033[1;30m"),
        RED_BOLD    ("\033[1;31m"),
        GREEN_BOLD  ("\033[1;32m"),
        YELLOW_BOLD ("\033[1;33m"),
        BLUE_BOLD   ("\033[1;34m"),
        MAGENTA_BOLD("\033[1;35m"),
        CYAN_BOLD   ("\033[1;36m"),
        WHITE_BOLD  ("\033[1;37m"),

        // Underline
        BLACK_UNDERLINED  ("\033[4;30m"),
        RED_UNDERLINED    ("\033[4;31m"),
        GREEN_UNDERLINED  ("\033[4;32m"),
        YELLOW_UNDERLINED ("\033[4;33m"),
        BLUE_UNDERLINED   ("\033[4;34m"),
        MAGENTA_UNDERLINED("\033[4;35m"),
        CYAN_UNDERLINED   ("\033[4;36m"),
        WHITE_UNDERLINED  ("\033[4;37m"),

        // Background
        BLACK_BACKGROUND  ("\033[40m"),
        RED_BACKGROUND    ("\033[41m"),
        GREEN_BACKGROUND  ("\033[42m"),
        YELLOW_BACKGROUND ("\033[43m"),
        BLUE_BACKGROUND   ("\033[44m"),
        MAGENTA_BACKGROUND("\033[45m"),
        CYAN_BACKGROUND   ("\033[46m"),
        WHITE_BACKGROUND  ("\033[47m"),

        // High Intensity
        BLACK_BRIGHT  ("\033[0;90m"),
        RED_BRIGHT    ("\033[0;91m"),
        GREEN_BRIGHT  ("\033[0;92m"),
        YELLOW_BRIGHT ("\033[0;93m"),
        BLUE_BRIGHT   ("\033[0;94m"),
        MAGENTA_BRIGHT("\033[0;95m"),
        CYAN_BRIGHT   ("\033[0;96m"),
        WHITE_BRIGHT  ("\033[0;97m"),

        // Bold High Intensity
        BLACK_BOLD_BRIGHT  ("\033[1;90m"),
        RED_BOLD_BRIGHT    ("\033[1;91m"),
        GREEN_BOLD_BRIGHT  ("\033[1;92m"),
        YELLOW_BOLD_BRIGHT ("\033[1;93m"),
        BLUE_BOLD_BRIGHT   ("\033[1;94m"),
        MAGENTA_BOLD_BRIGHT("\033[1;95m"),
        CYAN_BOLD_BRIGHT   ("\033[1;96m"),
        WHITE_BOLD_BRIGHT  ("\033[1;97m"),

        // High Intensity backgrounds
        BLACK_BACKGROUND_BRIGHT  ("\033[0;100m"),
        RED_BACKGROUND_BRIGHT    ("\033[0;101m"),
        GREEN_BACKGROUND_BRIGHT  ("\033[0;102m"),
        YELLOW_BACKGROUND_BRIGHT ("\033[0;103m"),
        BLUE_BACKGROUND_BRIGHT   ("\033[0;104m"),
        MAGENTA_BACKGROUND_BRIGHT("\033[0;105m"),
        CYAN_BACKGROUND_BRIGHT   ("\033[0;106m"),
        WHITE_BACKGROUND_BRIGHT  ("\033[0;107m");

        private final String code;

        Color(String code) { this.code = code; }
        @Override public String toString() { return code; }
    }
}
