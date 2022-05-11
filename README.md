# unix-shell-toolkit
A unix shell implementation

1. To compile mytoolkit.c
        $make mytoolkit

To run the code -

4. Run mytoolkit to start shell
        $./mytoolkit

5. Run mypwd, to find current directory
        $ mypwd

6. Run mycd to change it current directory
        $ mycd /home/grads

7. Run myexit to exit from mytoolkit
        $ myexit

8. Press CTRL-D or CTRL-C to exit from mytoolkit
        $ CTRL-D

9. Run a single command with or without parameters
        $ /bin/ls -l

10. Run command with single pipe
        $ /bin/cat mytoolkit.c | head -5

11. Run command with 2 pipes
        $ /bin/cat mytoolkit.c | head -5 | tail -2

12. Run command to output redirection
        $ /bin/ls -l > output.txt

13. Run command for input redirection
        $ /bin/wc < output.txt

14. Run command for Input and output redirection
	$ /bin/wc < mytoolkit.c > count.txt

15. Run mytimeout to timeout command execution
	$ mytimeout 5 /bin/ping 8.8.8.8
