plugins {
    id("com.android.application")
}

android {
    namespace = "com.androDashGL"
    compileSdk = 35
    ndkVersion = "27.3.13750724"

    defaultConfig {
        applicationId = "com.androDashGL"
        minSdk = 34
        targetSdk = 35
        versionCode = 1
        versionName = "0.1"

        externalNativeBuild {
            cmake {
                cppFlags("-std=c++17", "-Wall", "-Wextra")
                abiFilters("arm64-v8a")
            }
        }
    }

    externalNativeBuild {
        cmake {
            path("src/main/cpp/CMakeLists.txt")
            version = "3.22.1"
        }
    }

    buildTypes {
        release {
            isMinifyEnabled = false
        }
    }

    // No Java/Kotlin sources — suppress the empty source set warning
    sourceSets {
        getByName("main") {
            java.setSrcDirs(emptyList<String>())
        }
    }
}
