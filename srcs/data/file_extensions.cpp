#include "webserv.hpp"

using std::string;
using std::map;

static map<string,string> flip(map<string,string> my_map)
{
    map<string,string> flipped;

    for (map<string, string>::iterator i = my_map.begin(); i != my_map.end(); ++i)
        flipped[i->second] = i->first;
    return flipped;
}

map<string,string> fileExtension()
{
    map<string,string> extension;

    extension["text/html"] = "html";
    extension["text/html"] = "htm";
    extension["text/html"] = "shtml";
    extension["text/css"] = "css";
    extension["text/xml"] = "xml";
    extension["image/gif"] = "gif";
    extension["image/jpeg"] = "jpeg";
    extension["image/jpeg"] = "jpg";
    extension["application/javascript"] = "js";
    extension["application/atom+xml"] = "atom";
    extension["application/rss+xml"] = "rss";
    extension["text/mathml"] = "mml";
    extension["text/plain"] = "txt";
    extension["text/vnd.sun.j2me.app-descriptor"] = "jad";
    extension["text/vnd.wap.wml"] = "wml";
    extension["text/x-component"] = "htc";
    extension["image/png"] = "png";
    extension["image/svg+xml"] = "svg";
    extension["image/svg+xml"] = "svgz";
    extension["image/tiff"] = "tif";
    extension["image/tiff"] = "tiff";
    extension["image/vnd.wap.wbmp"] = "wbmp";
    extension["image/webp"] = "webp";
    extension["image/x-icon"] = "ico";
    extension["image/x-jng"] = "jng";
    extension["image/x-ms-bmp"] = "bmp";
    extension["font/woff"] = "woff";
    extension["font/woff2"] = "woff2";
    extension["application/java-archive"] = "jar";
    extension["application/java-archive"] = "war";
    extension["application/java-archive"] = "ear";
    extension["application/json"] = "json";
    extension["application/mac-binhex40"] = "hqx";
    extension["application/msword"] = "doc";
    extension["application/pdf"] = "pdf";
    extension["application/postscript"] = "ps";
    extension["application/postscript"] = "eps";
    extension["application/postscript"] = "ai";
    extension["application/rtf"] = "rtf";
    extension["application/vnd.apple.mpegurl"] = "m3u8";
    extension["application/vnd.google-earth.kml+xml"] = "kml";
    extension["application/vnd.google-earth.kmz"] = "kmz";
    extension["application/vnd.ms-excel"] = "xls";
    extension["application/vnd.ms-fontobject"] = "eot";
    extension["application/vnd.ms-powerpoint"] = "ppt";
    extension["application/vnd.oasis.opendocument.graphics"] = "odg";
    extension["application/vnd.oasis.opendocument.presentation"] = "odp";
    extension["application/vnd.oasis.opendocument.spreadsheet"] = "ods";
    extension["application/vnd.oasis.opendocument.text"] = "odt";
    extension["application/vnd.openxmlformats-officedocument.presentationml.presentation"] = "pptx";
    extension["application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"] = "xlsx";
    extension["application/vnd.openxmlformats-officedocument.wordprocessingml.document"] = "docx";
    extension["application/vnd.wap.wmlc"] = "wmlc";
    extension["application/x-7z-compressed"] = "7z";
    extension["application/x-cocoa"] = "cco";
    extension["application/x-java-archive-diff"] = "jardiff";
    extension["application/x-java-jnlp-file"] = "jnlp";
    extension["application/x-makeself"] = "run";
    extension["application/x-perl"] = "pl";
    extension["application/x-perl"] = "pm";
    extension["application/x-pilot"] = "prc";
    extension["application/x-pilot"] = "pdb";
    extension["application/x-rar-compressed"] = "rar";
    extension["application/x-redhat-package-manager"] = "rpm";
    extension["application/x-sea"] = "sea";
    extension["application/x-shockwave-flash"] = "swf";
    extension["application/x-stuffit"] = "sit";
    extension["application/x-tcl"] = "tcl";
    extension["application/x-tcl"] = "tk";
    extension["application/x-x509-ca-cert"] = "der";
    extension["application/x-x509-ca-cert"] = "pem";
    extension["application/x-x509-ca-cert"] = "crt";
    extension["application/x-xpinstall"] = "xpi";
    extension["application/xhtml+xml"] = "xhtml";
    extension["application/xspf+xml"] = "xspf";
    extension["application/zip"] = "zip";
    extension["application/octet-stream"] = "bin";
    extension["application/octet-stream"] = "exe";
    extension["application/octet-stream"] = "dll";
    extension["application/octet-stream"] = "deb";
    extension["application/octet-stream"] = "dmg";
    extension["application/octet-stream"] = "iso";
    extension["application/octet-stream"] = "img";
    extension["application/octet-stream"] = "msi";
    extension["application/octet-stream"] = "msp";
    extension["application/octet-stream"] = "msm";
    extension["audio/midi"] = "mid";
    extension["audio/midi"] = "midi";
    extension["audio/midi"] = "kar";
    extension["audio/mpeg"] = "mp3";
    extension["audio/ogg"] = "ogg";
    extension["audio/x-m4a"] = "m4a";
    extension["audio/x-realaudio"] = "ra";
    extension["video/3gpp"] = "3gpp";
    extension["video/3gpp"] = "3gp";
    extension["video/mp2t"] = "ts";
    extension["video/mp4"] = "mp4";
    extension["video/mpeg"] = "mpeg";
    extension["video/mpeg"] = "mpg";
    extension["video/quicktime"] = "mov";
    extension["video/webm"] = "webm";
    extension["video/x-flv"] = "flv";
    extension["video/x-m4v"] = "m4v";
    extension["video/x-mng"] = "mng";
    extension["video/x-ms-asf"] = "asx";
    extension["video/x-ms-asf"] = "asf";
    extension["video/x-ms-wmv"] = "wmv";
    extension["video/x-msvideo"] = "avi";

    return flip(extension);
}