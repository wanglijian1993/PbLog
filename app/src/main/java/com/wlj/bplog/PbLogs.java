package com.wlj.bplog;

import android.text.TextUtils;

import java.io.File;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

public class PbLogs {
    static {
        System.loadLibrary("bplog");
    }

    private static Map<String, List<FileLogger>> sFileLoggerMaps=new HashMap<>();

    private static int mFileTypeSize=5;

    private static int mMaxFileSize=5*1024*1024;
    private static String mLogRootPath;


   public static final void init(int fileTypeSize,int maxFileSize,String logRootPath)throws IOException{

       mFileTypeSize=fileTypeSize;
       mMaxFileSize=maxFileSize;
       mLogRootPath=logRootPath;

       File file=new File(logRootPath);
       if(!file.exists()){
           throw new IOException("logRootPath is not exists");
       }
       if(!file.isDirectory()){
           throw new IOException("logRootPath is not directory");
       }

       File[] logFiles=file.listFiles();
       if (logFiles == null) {
           return;
       }
       for(File logFile:logFiles){
           if(logFile.isFile()){

               String fileName=logFile.getName();
               String name=fileName.substring(0,fileName.indexOf("-"));
               List<FileLogger> fileLoggers=sFileLoggerMaps.get(name);
               if(fileLoggers==null){
                   fileLoggers=new ArrayList<>();
                   sFileLoggerMaps.put(name,fileLoggers);
               }
               FileLogger fileLogger=new FileLogger(logFile,mMaxFileSize);
               fileLoggers.add(fileLogger);

           }
       }

       Set<String> keys=sFileLoggerMaps.keySet();
       for(String key:keys){
           Collections.sort(sFileLoggerMaps.get(key));
       }

   }



   public static FileLogger getLogger(String modeName){

     if(TextUtils.isEmpty(mLogRootPath)){
         throw new NullPointerException("Log the parent directory is empty, please call the init method Settings");
     }

     List<FileLogger> fileLoggers=sFileLoggerMaps.get(modeName);

     if(fileLoggers==null){
         fileLoggers=new ArrayList<>();
         sFileLoggerMaps.put(modeName,fileLoggers);
     }
     return getFileLogger(fileLoggers,modeName);
   }

    private static FileLogger getFileLogger(List<FileLogger> fileLoggers, String modeName) {

       FileLogger fileLogger=null;
       if(isEmpty(fileLoggers)){
            //???????????????????????????????????????????????????
           fileLogger=new FileLogger(new File(getLogFileName(modeName)),mMaxFileSize);
           fileLoggers.add(fileLogger);

       }else{
           int typeSize=fileLoggers.size();
           fileLogger=fileLoggers.get(typeSize-1);
           //??????????????????????????????
           if(fileLogger.length()>=mMaxFileSize){
               //???????????????????????????
               fileLogger.release();
              //??????????????????????????????????????? mFileTypeSize,??????????????????log????????????
               if(typeSize==mFileTypeSize){
                   FileLogger deteleLogger=fileLoggers.remove(0);
                   deteleLogger.mLogFile.delete();
                   typeSize--;
               }
               //??????????????????
               fileLogger=new FileLogger(new File(getLogFileName(modeName)),mMaxFileSize);
               fileLoggers.add(fileLogger);

           }

       }

       return fileLogger;

   }
    /**
     * ???????????????????????????
     */
   public static String getLogFileName(String name){
       SimpleDateFormat sdf=new SimpleDateFormat("yyyy.MM.dd.HH.mm.ss");
       return mLogRootPath + "/" + name + "-" + sdf.format(System.currentTimeMillis()) + ".log";

   }
    private static final boolean isEmpty(Collection<?> collection) {
        return collection == null || collection.isEmpty();
    }
}
