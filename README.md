# arduino_VS1053_samplePlayer
Sample Player based on Adafruit's VS1053


https://trac.ffmpeg.org/wiki/AudioVolume

----recursivley convert
find ./ -iname '*.avi' -o -iname '*.mkv' -exec bash -c 'ffmpeg -i "{}" -c:v libx265 -preset medium -crf 28 -c:a aac "{}".mp4' \;

----recursively convert destructive:
find ./ -iname '*.avi' -o -iname '*.mkv' -exec bash -c 'ffmpeg -i "{}" -c:v libx265 -preset medium -crf 28 -c:a aac "{}".mp4 && rm "{}"' \;
