# unix-shell-toolkit
A unix shell implementation - mycd, mypwd, myexit, pipe, input and output redirection

To compile the code run below commands -

1. To compile mytoolkit.c
        $make mytoolkit

To run the code -

2. Run mytoolkit to start shell
        $./mytoolkit

3. Run mypwd, to find current directory
        $ mypwd

4. Run mycd to change it current directory
        $ mycd /home/grads

5. Run myexit to exit from mytoolkit
        $ myexit

6. Press CTRL-D or CTRL-C to exit from mytoolkit
        $ CTRL-D

7. Run a single command with or without parameters
        $ /bin/ls -l

8. Run command with single pipe
        $ /bin/cat mytoolkit.c | head -5

9. Run command with 2 pipes
        $ /bin/cat mytoolkit.c | head -5 | tail -2

10. Run command to output redirection
        $ /bin/ls -l > output.txt

11. Run command for input redirection
        $ /bin/wc < output.txt

12. Run command for Input and output redirection
	$ /bin/wc < mytoolkit.c > count.txt

13. Run mytimeout to timeout command execution
	$ mytimeout 5 /bin/ping 8.8.8.8
