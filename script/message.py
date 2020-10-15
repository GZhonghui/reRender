def py_print(message):
    print('\033[35m[Python]\033[0m',message,sep=' ',end='\n')

def main():
    py_print('Please dont run this script.')

if __name__=='__main__':
    main()