/****************************************************************************
Copyright (c) 2010-2013 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "CCFileUtils.h"
#include "cclog.h"
#include "CCString.h"
//#include "cocoa/CCDictionary.h"
//#include "cocoa/CCString.h"
//#include "CCSAXParser.h"
//#include "support/zip_support/unzip.h"
#include <stack>

using namespace std;

#define CC_SAFE_DELETE(p)            do { if(p) { delete (p); (p) = 0; } } while(0)
#define CC_SAFE_RELEASE(p)            do { if(p) { (p)->release(); } } while(0)
#define CC_BREAK_IF(cond)            if(cond) break

CCFileUtils* CCFileUtils::s_sharedFileUtils = NULL;

void CCFileUtils::purgeFileUtils()
{
    CC_SAFE_DELETE(s_sharedFileUtils);
}

CCFileUtils::CCFileUtils()
{
}

CCFileUtils::~CCFileUtils()
{
}

bool CCFileUtils::init()
{
    m_searchPathArray.push_back(m_strDefaultResRootPath);
    m_searchResolutionsOrderArray.push_back("");
    return true;
}

void CCFileUtils::purgeCachedEntries()
{
    m_fullPathCache.clear();
}

unsigned char* CCFileUtils::getFileData(const char* pszFileName, const char* pszMode, unsigned long * pSize)
{
    unsigned char * pBuffer = NULL;
    *pSize = 0;
    do
    {
        // read the file from hardware
        std::string fullPath = fullPathForFilename(pszFileName);
        FILE *fp = fopen(fullPath.c_str(), pszMode);
        CC_BREAK_IF(!fp);
        
        fseek(fp,0,SEEK_END);
        *pSize = ftell(fp);
        fseek(fp,0,SEEK_SET);
        pBuffer = new unsigned char[*pSize];
        *pSize = fread(pBuffer,sizeof(unsigned char), *pSize,fp);
        fclose(fp);
    } while (0);
    
    if (! pBuffer)
    {
        std::string msg = "Get data from file(";
        msg.append(pszFileName).append(") failed!");
        
        CCLOG(msg.c_str());
    }
    return pBuffer;
}
//
//unsigned char* CCFileUtils::getFileDataFromZip(const char* pszZipFilePath, const char* pszFileName, unsigned long * pSize)
//{
//    unsigned char * pBuffer = NULL;
//    unzFile pFile = NULL;
//    *pSize = 0;
//
//    do 
//    {
//        CC_BREAK_IF(!pszZipFilePath || !pszFileName);
//        CC_BREAK_IF(strlen(pszZipFilePath) == 0);
//
//        pFile = unzOpen(pszZipFilePath);
//        CC_BREAK_IF(!pFile);
//
//        int nRet = unzLocateFile(pFile, pszFileName, 1);
//        CC_BREAK_IF(UNZ_OK != nRet);
//
//        char szFilePathA[260];
//        unz_file_info FileInfo;
//        nRet = unzGetCurrentFileInfo(pFile, &FileInfo, szFilePathA, sizeof(szFilePathA), NULL, 0, NULL, 0);
//        CC_BREAK_IF(UNZ_OK != nRet);
//
//        nRet = unzOpenCurrentFile(pFile);
//        CC_BREAK_IF(UNZ_OK != nRet);
//
//        pBuffer = new unsigned char[FileInfo.uncompressed_size];
//        int CC_UNUSED nSize = unzReadCurrentFile(pFile, pBuffer, FileInfo.uncompressed_size);
//        CCAssert(nSize == 0 || nSize == (int)FileInfo.uncompressed_size, "the file size is wrong");
//
//        *pSize = FileInfo.uncompressed_size;
//        unzCloseCurrentFile(pFile);
//    } while (0);
//
//    if (pFile)
//    {
//        unzClose(pFile);
//    }
//
//    return pBuffer;
//}

std::string CCFileUtils::getNewFilename(const char* pszFileName)
{
    const char* pszNewFileName = NULL;
    // in Lookup Filename dictionary ?
    pszNewFileName = pszFileName;
    return pszNewFileName;
}

std::string CCFileUtils::getPathForFilename(const std::string& filename, const std::string& resolutionDirectory, const std::string& searchPath)
{
    std::string file = filename;
    std::string file_path = "";
    size_t pos = filename.find_last_of("/");
    if (pos != std::string::npos)
    {
        file_path = filename.substr(0, pos+1);
        file = filename.substr(pos+1);
    }
    
    // searchPath + file_path + resourceDirectory
    std::string path = searchPath;
    path += file_path;
    path += resolutionDirectory;
    
    path = getFullPathForDirectoryAndFilename(path, file);
    
    //CCLOG("getPathForFilename, fullPath = %s", path.c_str());
    return path;
}


std::string CCFileUtils::fullPathForFilename(const char* pszFileName)
{
    std::string strFileName = pszFileName;
    if (isAbsolutePath(pszFileName))
    {
        //CCLOG("Return absolute path( %s ) directly.", pszFileName);
        return pszFileName;
    }
    
    // Already Cached ?
    std::map<std::string, std::string>::iterator cacheIter = m_fullPathCache.find(pszFileName);
    if (cacheIter != m_fullPathCache.end())
    {
        //CCLOG("Return full path from cache: %s", cacheIter->second.c_str());
        return cacheIter->second;
    }
    
    // Get the new file name.
    std::string newFilename = getNewFilename(pszFileName);
    
    string fullpath = "";
    
    for (std::vector<std::string>::iterator searchPathsIter = m_searchPathArray.begin();
         searchPathsIter != m_searchPathArray.end(); ++searchPathsIter) {
        for (std::vector<std::string>::iterator resOrderIter = m_searchResolutionsOrderArray.begin();
             resOrderIter != m_searchResolutionsOrderArray.end(); ++resOrderIter) {
            
            //CCLOG("\n\nSEARCHING: %s, %s, %s", newFilename.c_str(), resOrderIter->c_str(), searchPathsIter->c_str());
            
            fullpath = this->getPathForFilename(newFilename, *resOrderIter, *searchPathsIter);
            
            if (fullpath.length() > 0)
            {
                // Using the filename passed in as key.
                m_fullPathCache.insert(std::pair<std::string, std::string>(pszFileName, fullpath));
                //CCLOG("Returning path: %s", fullpath.c_str());
                return fullpath;
            }
        }
    }
    
    // The file wasn't found, return the file name passed in.
    return pszFileName;
}

const char* CCFileUtils::fullPathFromRelativeFile(const char *pszFilename, const char *pszRelativeFile)
{
    std::string relativeFile = pszRelativeFile;
    CCString *pRet = CCString::create("");
    pRet->m_sString = relativeFile.substr(0, relativeFile.rfind('/')+1);
    pRet->m_sString += getNewFilename(pszFilename);
    return pRet->getCString();
}

void CCFileUtils::setSearchResolutionsOrder(const std::vector<std::string>& searchResolutionsOrder)
{
    bool bExistDefault = false;
    m_searchResolutionsOrderArray.clear();
    for (std::vector<std::string>::const_iterator iter = searchResolutionsOrder.begin(); iter != searchResolutionsOrder.end(); ++iter)
    {
        std::string resolutionDirectory = *iter;
        if (!bExistDefault && resolutionDirectory == "")
        {
            bExistDefault = true;
        }
        
        if (resolutionDirectory.length() > 0 && resolutionDirectory[resolutionDirectory.length()-1] != '/')
        {
            resolutionDirectory += "/";
        }
        
        m_searchResolutionsOrderArray.push_back(resolutionDirectory);
    }
    if (!bExistDefault)
    {
        m_searchResolutionsOrderArray.push_back("");
    }
}

void CCFileUtils::addSearchResolutionsOrder(const char* order)
{
    m_searchResolutionsOrderArray.push_back(order);
}

const std::vector<std::string>& CCFileUtils::getSearchResolutionsOrder()
{
    return m_searchResolutionsOrderArray;
}

const std::vector<std::string>& CCFileUtils::getSearchPaths()
{
    return m_searchPathArray;
}

void CCFileUtils::setSearchPaths(const std::vector<std::string>& searchPaths)
{
    bool bExistDefaultRootPath = false;
    
    m_searchPathArray.clear();
    for (std::vector<std::string>::const_iterator iter = searchPaths.begin(); iter != searchPaths.end(); ++iter)
    {
        std::string strPrefix;
        std::string path;
        if (!isAbsolutePath(*iter))
        { // Not an absolute path
            strPrefix = m_strDefaultResRootPath;
        }
        path = strPrefix+(*iter);
        if (path.length() > 0 && path[path.length()-1] != '/')
        {
            path += "/";
        }
        if (!bExistDefaultRootPath && path == m_strDefaultResRootPath)
        {
            bExistDefaultRootPath = true;
        }
        m_searchPathArray.push_back(path);
    }
    
    if (!bExistDefaultRootPath)
    {
        //CCLOG("Default root path doesn't exist, adding it.");
        m_searchPathArray.push_back(m_strDefaultResRootPath);
    }
}

void CCFileUtils::addSearchPath(const char* path_)
{
    std::string strPrefix;
    std::string path(path_);
    if (!isAbsolutePath(path))
    { // Not an absolute path
        strPrefix = m_strDefaultResRootPath;
    }
    path = strPrefix + path;
    if (path.length() > 0 && path[path.length()-1] != '/')
    {
        path += "/";
    }
    m_searchPathArray.push_back(path);
}

//void CCFileUtils::setFilenameLookupDictionary(CCDictionary* pFilenameLookupDict)
//{
//    //CC_SAFE_RELEASE(m_pFilenameLookupDict);
//    //m_pFilenameLookupDict = pFilenameLookupDict;
//    //CC_SAFE_RETAIN(m_pFilenameLookupDict);
//}

//void CCFileUtils::loadFilenameLookupDictionaryFromFile(const char* filename)
//{
//    std::string fullPath = this->fullPathForFilename(filename);
//    if (fullPath.length() > 0)
//    {
//        CCDictionary* pDict = CCDictionary::createWithContentsOfFile(fullPath.c_str());
//        if (pDict)
//        {
//            CCDictionary* pMetadata = (CCDictionary*)pDict->objectForKey("metadata");
//            int version = ((CCString*)pMetadata->objectForKey("version"))->intValue();
//            if (version != 1)
//            {
//                CCLOG("cocos2d: ERROR: Invalid filenameLookup dictionary version: %ld. Filename: %s", (long)version, filename);
//                return;
//            }
//            setFilenameLookupDictionary((CCDictionary*)pDict->objectForKey("filenames"));
//        }
//    }
//}

std::string CCFileUtils::getFullPathForDirectoryAndFilename(const std::string& strDirectory, const std::string& strFilename)
{
    std::string ret = strDirectory+strFilename;
    if (!isFileExist(ret)) {
        ret = "";
    }
    return ret;
}

bool CCFileUtils::isAbsolutePath(const std::string& strPath)
{
    return strPath[0] == '/' ? true : false;
}

//////////////////////////////////////////////////////////////////////////
// Notification support when getFileData from invalid file path.
//////////////////////////////////////////////////////////////////////////
static bool s_bPopupNotify = true;

void CCFileUtils::setPopupNotify(bool bNotify)
{
    s_bPopupNotify = bNotify;
}

bool CCFileUtils::isPopupNotify()
{
    return s_bPopupNotify;
}

