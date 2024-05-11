#pragma once

#include "misc.h"
#include <string>
#include <vector>
#include <set>
#include <sys/types.h>

namespace android {

    /*
     * This provides vector-style access to a directory.  We do this rather
     * than modeling opendir/readdir access because it's simpler and the
     * nature of the operation requires us to have all data on hand anyway.
     *
     * The list of files will be sorted in ascending order by ASCII value.
     *
     * The contents are populated by our friend, the AssetManager.
     */
    class AssetDir {
    public:
        AssetDir(void)
            : mFileInfo(NULL)
        {}
        virtual ~AssetDir(void) {
            delete mFileInfo;
        }

        /*
         * Vector-style access.
         */
        size_t getFileCount(void) { return mFileInfo->size(); }
        const std::string& getFileName(int idx) {
            if (idx < mFileInfo->size()) {
                auto it = mFileInfo->begin();
                std::advance(it, idx);
                return it->getFileName();
            }
            return std::string("");
        }
        const std::string& getSourceName(int idx) {
            if (idx < mFileInfo->size()) {
				auto it = mFileInfo->begin();
				std::advance(it, idx);
				return it->getSourceName();
			}
            return std::string("");
        }

        /*
         * Get the type of a file (usually regular or directory).
         */
        FileType getFileType(int idx) {
            if (idx < mFileInfo->size()) {
				auto it = mFileInfo->begin();
				std::advance(it, idx);
				return it->getFileType();
			}
            return kFileTypeUnknown;
        }

    private:
        /* these operations are not implemented */
        AssetDir(const AssetDir& src);
        const AssetDir& operator=(const AssetDir& src);

        friend class AssetManager;
        friend class AssetManager2;

        /*
         * This holds information about files in the asset hierarchy.
         */
        class FileInfo {
        public:
            FileInfo(void) {}
            explicit FileInfo(const std::string& path)      // useful for e.g. svect.indexOf
                : mFileName(path), mFileType(kFileTypeUnknown)
            {}
            ~FileInfo(void) {}
            FileInfo(const FileInfo& src) {
                copyMembers(src);
            }
            const FileInfo& operator= (const FileInfo& src) {
                if (this != &src)
                    copyMembers(src);
                return *this;
            }

            void copyMembers(const FileInfo& src) {
                mFileName = src.mFileName;
                mFileType = src.mFileType;
                mSourceName = src.mSourceName;
            }

            /* need this for SortedVector; must compare only on file name */
            bool operator< (const FileInfo& rhs) const {
                return mFileName < rhs.mFileName;
            }

            /* used by AssetManager */
            bool operator== (const FileInfo& rhs) const {
                return mFileName == rhs.mFileName;
            }

            void set(const std::string& path, FileType type) {
                mFileName = path;
                mFileType = type;
            }

            const std::string& getFileName(void) const { return mFileName; }
            void setFileName(const std::string& path) { mFileName = path; }

            FileType getFileType(void) const { return mFileType; }
            void setFileType(FileType type) { mFileType = type; }

            const std::string& getSourceName(void) const { return mSourceName; }
            void setSourceName(const std::string& path) { mSourceName = path; }

            /*
             * Handy utility for finding an entry in a sorted vector of FileInfo.
             * Returns the index of the matching entry, or -1 if none found.
             */
            static int findEntry(const std::set<FileInfo>* pVector,
                const std::string& fileName);

        private:
            std::string mFileName;      // filename only
            FileType    mFileType;      // regular, directory, etc

            std::string mSourceName;    // currently debug-only
        };

        /* AssetManager uses this to initialize us */
        void setFileList(std::set<FileInfo>* list) { mFileInfo = list; }

        std::set<FileInfo>* mFileInfo;
    };

}; // namespace android