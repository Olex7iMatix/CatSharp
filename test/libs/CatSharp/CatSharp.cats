pack CatSharp {
    class Console {
        
        void Log(msg) {
            log("[CatSharp]", msg);
        };

        void LogInfo(msg) {
            log("[INFO]", msg);
        };

        void LogError(msg) {
            log("[ERROR]", msg);
        };

        void logUwU(uwu) {
            if (uwu == "UwU") {
                log ("UwU");
            } else {
                log("Not UwU");
            }
        };

    };
};