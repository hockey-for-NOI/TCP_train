# Homework for TCP

#### CST53 He Qi 2015011299

## Introduction

Use TCP protocol to implement a file uploader/downloader.

## Protocol

|Name|Length(in bytes)|Description|
|:-:|--:|:--|
|opcode|4|Read=0xFACECAFE, Write=0xCAFEFACE.|
|namelen|4|Length of filename in bytes. At most 100.|
|name|vary|Filename(without trailing \\0).|
|filelen|4|Length of file in bytes. For upload, at most 1048576.|
|file|vary|The content of file.|
|crc32|4|CRC32 checksum.|

For uploading, the client provide all these 6 parts.

For downloading, the client provide the first 3 parts, and server will return the following 3 parts.
