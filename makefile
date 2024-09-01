
#指定最终目标 => 最终生成的可执行的文件名 main
TARGET:=main

#指定编译器 arm-linux-gcc / gcc
CC:=arm-linux-gcc

#指定头文件的搜索路径 
Include:=-I./
Include+=-I./inc/ #指定源代码头文件搜索路径
Include+=-I./include/ #指定库对应的头文件的路径

#指定库文件的搜索路径 
LIB:=-L ./
LIB+=-L ./libs/ 
   
#指定库的名字 
LIB_NAME:=-lm -lpthread -ljpeg
#LIB_NAME+=-lstack
#LIB_NAME+=-lqueue
    
#获取各个路径下面的.c文件 
Source:=$(wildcard ./*.c)
Source+=$(wildcard ./src/*.c) 

#将所有的.c，替换为所有的.o ，切记:patsubst 不能够有空格
Dest:=$(patsubst %.c,%.o,$(Source))

$(TARGET):$(Dest)
	$(CC) $^ -o $@ $(Include) $(LIB) $(LIB_NAME)
	cp $(TARGET) /home/china/tftp
	mv $(TARGET) ./bin/
	
	
%.o:%.c
	$(CC) -c $< -o $@ $(Include) $(LIB)

clean:
	rm $(Dest)
	rm ./bin/$(TARGET)
