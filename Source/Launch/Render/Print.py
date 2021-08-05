import time

def Print(Message:str):
    #Head
    print('[*Python]',end='')
    
    Now=time.localtime(time.time())
    print(' [%02d:%02d:%02d] >>'%(Now.tm_hour,Now.tm_min,Now.tm_sec),end='')
    
    print(Message,end='')

    #Tail
    print('')

def GetBar(x,total:int,length:int):
    x=max(min(x,total),0)
    blocks=int(x*length/total)
    emptys=length-blocks
    return '[%06.2f%%]'%(x*100/total)+' ['+'#'*blocks+'-'*emptys+']'