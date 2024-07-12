## B조(BestReviewer) SSD 프로젝트

# 빌드
- visual studio에서 SSD_Test_Project.sln 를 오픈합니다.
- Release, x64로 설정하고 '솔루션 다시빌드'를 합니다.

# 실행
- Root 폴더에서 Scripts/ 폴더 아래의 runlist.lst와 스크립트 파일들을 Root폴더 아래의 x64/Release 폴더로 복사합니다.
- Root폴더 아래의 x64/Release 폴더에서 command prompt를 오픈합니다.
>Shell.exe runlist.lst

# 명령어 
- write {LBA} {Data}
- read {LBA}
- exit
- help
- fullwrite {Data}
- fullread
- testapp1
- testapp2
- erase {LBA} {Size}
- erase_range {LBA} {LBA}
- flush
>{LBA} = {x is an integer | 0 <= x <= 99}
 
>{Data} = {0x[0-9A-F]}
