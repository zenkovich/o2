#include "o2/stdafx.h"

#ifdef PLATFORM_MAC

#import <Cocoa/Cocoa.h>

#include "o2/Utils/FileSystem/FileSystem.h"

#include "o2/Application/Application.h"
#include "o2/Utils/Debug/Debug.h"
#include "o2/Utils/Debug/Log/LogStream.h"

namespace o2
{
	NSString* GetBundleDirectory()
	{
		return [[[[NSBundle mainBundle] bundlePath] stringByDeletingLastPathComponent] stringByAppendingString:@"/"];
	}
	
	NSString* GetBundleRelativePath(const String& path)
	{
		return [[[[[NSBundle mainBundle] bundlePath] stringByDeletingLastPathComponent]
				 stringByAppendingString:@"/"] stringByAppendingString:[NSString stringWithUTF8String:path.Data()]];
	}
	
	const String& FileSystem::GetBundlePath() const
	{
		static String bundlePath = [GetBundleDirectory() UTF8String];
		return bundlePath;
	}
	
	FolderInfo FileSystem::GetFolderInfo(const String& path) const
	{
		FolderInfo res;
		res.path = path;
		
		@autoreleasepool
		{
			NSString* searchPath = GetBundleRelativePath(path);
			
			@try
			{
				NSError* err = NULL;
				NSArray* dirs = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:searchPath error:&err];
				if (err) {
					mInstance->mLog->Error("Failed GetPathInfo: Error opening directory " + path + ", error:" + [[err description] UTF8String]);
				}
				
				for (id obj in dirs)
				{
					String filePath = path + [(NSString*)obj UTF8String];
					
					BOOL isDir = NO;
					BOOL exists = [[NSFileManager defaultManager] fileExistsAtPath:[searchPath stringByAppendingString:obj] isDirectory:&isDir];

					if (isDir || ! exists)
						res.folders.Add(GetFolderInfo(filePath + "/"));
					else
						res.files.Add(GetFileInfo(filePath));
				};
			}
			@catch (NSException *exception)
			{
				mInstance->mLog->Error("Failed GetPathInfo: Error opening directory " + path + ", exception:" + [[exception reason] UTF8String]);
			}
		}
		
		return res;
	}

	bool FileSystem::FileCopy(const String& source, const String& dest) const
	{
		FileDelete(dest);
		FolderCreate(ExtractPathStr(dest));
		
		bool result = false;
		
		@autoreleasepool
		{
			@try
			{
				NSString* sourcePath = GetBundleRelativePath(source);
				NSString* destPath = GetBundleRelativePath(dest);
				
				NSError* err = NULL;
				result = [[NSFileManager defaultManager] copyItemAtPath:sourcePath toPath:destPath error:&err];
				if (err) {
					mInstance->mLog->Error("Failed FileCopy: Error copying from " + source + " to " + dest + ", error:" + [[err description] UTF8String]);
				}
			}
			@catch (NSException *exception)
			{
				mInstance->mLog->Error((String)"Failed FileCopy, exception:" + [[exception reason] UTF8String]);
			}
		}
		
		return result;
	}

	bool FileSystem::FileDelete(const String& file) const
	{
		if (!IsFileExist(file))
			return false;

		bool result = false;
		
		@autoreleasepool
		{
			@try
			{
				NSString* filePath = GetBundleRelativePath(file);
				NSError* err = NULL;
				result = [[NSFileManager defaultManager] removeItemAtPath:filePath error:&err];
				if (err) {
					mInstance->mLog->Error("Failed FileDelete: Error removing file from " + file + ", error:" + [[err description] UTF8String]);
				}
			}
			@catch (NSException *exception)
			{
				mInstance->mLog->Error((String)"Failed FileDelete, exception:" + [[exception reason] UTF8String]);
			}
		}
		
		return result;
	}

	bool FileSystem::FileMove(const String& source, const String& dest) const
	{
		String destFolder = GetParentPath(dest);
		
		if (!IsFolderExist(destFolder))
			FolderCreate(destFolder);
		
		bool result = false;
		
		@autoreleasepool
		{
			@try
			{
				NSString* sourcePath = GetBundleRelativePath(source);
				NSString* destPath = GetBundleRelativePath(dest);
				
				NSError* err = NULL;
				result = [[NSFileManager defaultManager] moveItemAtPath:sourcePath toPath:destPath error:&err];
				if (err) {
					mInstance->mLog->Error("Failed FileCopy: Moving file from " + source + " to " + dest + ", error:" + [[err description] UTF8String]);
				}
			}
			@catch (NSException *exception)
			{
				mInstance->mLog->Error((String)"Failed FileMove, exception:" + [[exception reason] UTF8String]);
			}
		}
		
		return result;
	}

	FileInfo FileSystem::GetFileInfo(const String& path) const
	{
		FileInfo res;
		res.path = path;
		
		String extension = path.SubStr(path.FindLast(".") + 1);
		
		@autoreleasepool
		{
			NSDictionary* fileAttribs;
		
			@try
			{
				NSString* npath = GetBundleRelativePath(path);
				NSError* err = NULL;
				fileAttribs = [[NSFileManager defaultManager] attributesOfItemAtPath:npath error:&err];
				if (err) {
					mInstance->mLog->Error("Failed FileCopy: Error getting file attributes from " + path + ", error:" + [[err description] UTF8String]);
				}
			}
			@catch (NSException *exception)
			{
				mInstance->mLog->Error((String)"Failed GetFileInfo, exception:" + [[exception reason] UTF8String]);
			}
			
			NSDate *creationDate = [fileAttribs objectForKey:NSFileCreationDate];
			NSDate *modificationDate = [fileAttribs objectForKey:NSFileModificationDate];
			
			NSCalendar *gregorianCal = [[NSCalendar alloc] initWithCalendarIdentifier:NSCalendarIdentifierGregorian];
			NSDateComponents *creationDateComps = [gregorianCal components: (NSCalendarUnitSecond | NSCalendarUnitMinute |
																			 NSCalendarUnitHour | NSCalendarUnitDay |
																			 NSCalendarUnitMonth | NSCalendarUnitYear)
																  fromDate: creationDate];
			
			NSDateComponents *modificationDateComps = [gregorianCal components: (NSCalendarUnitSecond | NSCalendarUnitMinute |
																				 NSCalendarUnitHour | NSCalendarUnitDay |
																				 NSCalendarUnitMonth | NSCalendarUnitYear)
																	  fromDate: modificationDate];
			
			res.createdDate = TimeStamp((int)[creationDateComps second], (int)[creationDateComps minute], (int)[creationDateComps hour],
										(int)[creationDateComps day], (int)[creationDateComps month], (int)[creationDateComps year]);
			
			res.editDate = TimeStamp((int)[modificationDateComps second], (int)[modificationDateComps minute], (int)[modificationDateComps hour],
									 (int)[modificationDateComps day], (int)[modificationDateComps month], (int)[modificationDateComps year]);
			
			res.accessDate = res.editDate;
			
			res.size = (UInt)[fileAttribs fileSize];
		}
		return res;
	}

	bool FileSystem::SetFileEditDate(const String& path, const TimeStamp& time) const
	{
		bool result = false;
		
		@autoreleasepool
		{
			@try
			{
				NSString* filePath = GetBundleRelativePath(path);
				
				NSCalendar *calendar = [[NSCalendar alloc] initWithCalendarIdentifier:NSCalendarIdentifierGregorian];
				NSDateComponents *components = [[NSDateComponents alloc] init];
				[components setYear:time.mYear];
				[components setMonth:time.mMonth];
				[components setDay:time.mDay];
				[components setHour:time.mHour];
				[components setMinute:time.mMinute];
				[components setSecond:time.mSecond];
				
				NSDate* date = [calendar dateFromComponents:components];
				NSDictionary* attr = [NSDictionary dictionaryWithObjectsAndKeys: date, NSFileModificationDate, NULL];
				
				NSError* err = NULL;
				result = [[NSFileManager defaultManager] setAttributes:attr ofItemAtPath:filePath error:&err];
				if (err) {
					mInstance->mLog->Error("Failed FileCopy: Error setting file attributes to " + path + ", error:" + [[err description] UTF8String]);
				}
			}
			@catch (NSException *exception)
			{
				mInstance->mLog->Error((String)"Failed SetFileEditDate, exception:" + [[exception reason] UTF8String]);
			}
		}
		
		return result;
	}

	bool FileSystem::FolderCreate(const String& path, bool recursive /*= true*/) const
	{
		if (IsFolderExist(path))
			return true;
		
		bool result = false;
		
		@autoreleasepool
		{
			@try
			{
				NSString* filePath = GetBundleRelativePath(path);
				NSError* err = NULL;
				result = [[NSFileManager defaultManager] createDirectoryAtPath:filePath withIntermediateDirectories:recursive attributes:nil error:&err];
				if (err) {
					mInstance->mLog->Error("Failed FileCopy: Error creating folder at " + path + ", error:" + [[err description] UTF8String]);
				}
			}
			@catch (NSException *exception)
			{
				mInstance->mLog->Error((String)"Failed FolderCreate, exception:" + [[exception reason] UTF8String]);
			}
		}
		
		return result;
	}

	bool FileSystem::FolderCopy(const String& from, const String& to) const
	{
		if (!IsFolderExist(from) || !IsFolderExist(to))
			return false;
		
		bool result = false;
		
		@autoreleasepool
		{
			@try
			{
				NSString* sourcePath = GetBundleRelativePath(from);
				NSString* destPath = GetBundleRelativePath(to);
				
				NSError* err = NULL;
				result = [[NSFileManager defaultManager] copyItemAtPath:sourcePath toPath:destPath error:&err];
				if (err) {
					mInstance->mLog->Error("Failed FileCopy: Error copying folder from " + from + " to " + to + ", error:" + [[err description] UTF8String]);
				}
			}
			@catch (NSException *exception)
			{
				mInstance->mLog->Error((String)"Failed FolderCopy, exception:" + [[exception reason] UTF8String]);
			}
		}
		
		return result;
	}

	bool FileSystem::FolderRemove(const String& path, bool recursive /*= true*/) const
	{
		if (!IsFolderExist(path))
			return false;
		
		bool result = false;
		
		@autoreleasepool
		{
			@try
			{
				NSString* filePath = GetBundleRelativePath(path);
				NSError* err = NULL;
				result = [[NSFileManager defaultManager] removeItemAtPath:filePath error:&err];
				if (err) {
					mInstance->mLog->Error("Failed FileCopy: Error removing folder at " + path + ", error:" + [[err description] UTF8String]);
				}
			}
			@catch (NSException *exception)
			{
				mInstance->mLog->Error((String)"Failed FileDelete, exception:" + [[exception reason] UTF8String]);
			}
		}
		
		return result;
	}

	bool FileSystem::Rename(const String& old, const String& newPath) const
	{
		int res = rename(old, newPath);
		return res == 0;
	}

	bool FileSystem::IsFolderExist(const String& path) const
	{
		@autoreleasepool
		{
			NSString* folderPath = GetBundleRelativePath(path);
			BOOL isDir = NO;
			BOOL isFile = [[NSFileManager defaultManager] fileExistsAtPath:folderPath isDirectory:&isDir];
		
			return isDir && isFile == YES;
		}
	}

	bool FileSystem::IsFileExist(const String& path) const
	{
		@autoreleasepool
		{
			NSString* filePath = GetBundleRelativePath(path);
			BOOL isDir = NO;
			BOOL isFile = [[NSFileManager defaultManager] fileExistsAtPath:filePath isDirectory:&isDir];
			
			return isDir == NO && isFile == YES;
		}
	}
	
	String FileSystem::GetPathRelativeToPath(const String& from, const String& to)
	{
		return to;
	}
	
	String FileSystem::CanonicalizePath(const String& path)
	{
		return path;
	}
}

#endif // PLATFORM_WINDOWS
