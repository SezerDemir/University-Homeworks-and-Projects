(defvar *token-check-list* nil)
(defvar *token-list* nil)
;;if a line contains full of whitespaces it returns F otherwise T.
(defun check-fullwhitespace-line (line)
    (let ((flag nil) )
        (loop for c across line do (if (not (eq c #\Space)) (setq flag T) ) )
        flag
    )
)
;;reads file and stores all content in a single string
(defun getFile(filename) 
    (let ((in (open filename :if-does-not-exist nil))(str ""))
        (when in
            (loop for line = (read-line in nil)
                while line do (if (eq (check-fullwhitespace-line line) T) (setq str (concatenate 'string str (removeExtraWhiteSpaces (addWhiteSpaces line)) ))) do(if (not (eq (search-comment line) -1)) (setq str (concatenate  'string str "                            ;;&& " )))  do(if (eq (check-fullwhitespace-line line) T) (setq str (concatenate  'string str " " )) )  )
            (close in)
        )
        (setq str (concatenate 'string str " ")  )
        str
    )
)

;;if there isn't a comment sign in string it return -1 otherwise index of the beginning of the sign
(defun search-comment (str)
    (let ((index 0)(counter 0)(final-index -1)(flag 0))
        (loop for n across str 
            do(if (eq n #\;) (setq counter (+ counter 1)) (setq counter 0))
            do(cond ((and (eq counter 2) (equal flag 0)) (setq final-index (- index 1)) (setq flag 1)))
            do(setq index (+ index 1))
        )
        final-index
    )
)

;;to check paramater is a operator, if it is it returns T otherwise nil
(defun isOperator (operator)
    (setq operatorList '(#\+ #\- #\/ #\* #\( #\) #\"))
    (cond ((eq (find operator operatorList) nil) nil) (T T))
)
;;it adds additional white spaces for ; and ( or ) signs 
(defun addWhiteSpaces (code)
    (let ((tempCode "")(quo-flag 0) (quo-flag2 0) (index 0) (temp-c #\1) (finalCode "") (quo-added-flag 0) (op-added-flag 0) )
        (setq whiteSpaceList '(#\( #\) #\"))
        (loop for c across code
            do(cond ((eq c #\;) (setq quo-flag 1) ) (T (setq quo-flag 0) (setq quo-flag2 0) ))
            do(cond ((and (eq quo-flag 1) (< (+ index 1) (length code))) (setq temp-c (char code (+ index 1)  )) ) )
            do(cond ( (and (and (eq quo-flag 1) (eq temp-c #\;)) (eq quo-flag2 0) ) (setq finalCode (concatenate 'string finalCode " ;" )) (setq quo-added-flag 1) (setq quo-flag2 1) ) (T (setq quo-added-flag 0)) )
            do(setq index (+ index 1) )
            do(setq temp-c #\1)
            do(cond ((and (not (eq (find c whiteSpaceList) nil)) (eq quo-added-flag 0) ) (setq op-added-flag 1) (setq finalCode (concatenate 'string finalCode (list #\Space) (list c) (list #\Space)))) (T (setq op-added-flag 0)) ) 
            do(if (and (eq quo-added-flag 0) (eq op-added-flag 0) ) (setq finalCode (concatenate 'string finalCode (list c) )) )
        )
        finalCode
    )
)
;; removes extra whitespaces those are in a sequance
(defun removeExtraWhiteSpaces (code)
    (let ((counter 0)) 
        (setq tempCode "")
        (loop for c across code 
            do(cond ((eq c #\Space) (setq counter (+ counter 1))) (T (setq counter 0)))
            do(if (< counter 2) (setq tempCode (concatenate 'string tempCode (list c))))
        )
        tempCode
    )
    
)
;;adds whitespaces, that given number, before the word 
(defun add-whitespace-before-word (str num)
    (let ((word ""))
        (loop for n from 1 to num do(setq word (concatenate 'string word (list #\Space)))
        )
        (setq word (concatenate 'string word str))
        word
    )
)
;;it seperates words in code that should be a token and add them into token-check-list
(defun fill-token-check-list (str)
    (let ((white-space-counter 0) (white-space-flag 1) (word "") (flag-word 0))
        (loop for c across str
            do(cond ((not (eq c #\Space)) (setq white-space-flag 1) ) (T (setq white-space-counter (+ white-space-counter 1)) (setq white-space-flag 0) (setq flag-word 0)) 
            )
            do(cond ( (eq white-space-flag 1) (setq word (concatenate 'string word (list c)))  (setq flag-word 1) )
            )      
            do(cond ( (and (not (string-equal word "")) (eq white-space-flag 0) ) (setq word (add-whitespace-before-word word (- white-space-counter 1))) (setq *token-check-list* (append *token-check-list* (list word))) (setq word "") (setq white-space-counter 1) )
            )
        )
        *token-check-list*
    )
)
;;trims left and right side whitespaces from each element of token-check-list if it is not a comment line end-sign
(defun remove-extra-whitespaces-from-token-list (tokenList)
    (let ((counter 0) (newList nil) (new-word ""))
        (loop for word in tokenList 
            do(loop for c across word 
                do(if (eq c #\Space) (setq counter (+ counter 1)) (setq new-word (concatenate 'string new-word (list c))))
            )
            do(if (> counter 5) (setq newList (append newList (list word))) (setq newList (append newList (list new-word))) 
            )
            do(setq new-word "")
            do(setq counter 0)
        )
        newList
    )
)

;;checks word is a keyword or not. If it is, it return the associated token otherwise empty string
(defun check-keyword-token (word)
    (let ((token "") )
        (cond
            ((string-equal word "and") (setq token "KW_AND"))
            ((string-equal word "or") (setq token "KW_OR"))
            ((string-equal word "not") (setq token "KW_NOT"))
            ((string-equal word "equal") (setq token "KW_EQUAL"))
            ((string-equal word "less") (setq token "KW_LESS"))
            ((string-equal word "nil") (setq token "KW_NIL"))
            ((string-equal word "list") (setq token "KW_LIST"))
            ((string-equal word "append") (setq token "KW_APPEND"))
            ((string-equal word "concat") (setq token "KW_CONCAT"))
            ((string-equal word "set") (setq token "KW_SET"))
            ((string-equal word "deffun") (setq token "KW_DEFFUN"))
            ((string-equal word "for") (setq token "KW_FOR"))
            ((string-equal word "if") (setq token "KW_IF"))
            ((string-equal word "exit") (setq token "KW_EXIT"))
            ((string-equal word "load") (setq token "KW_LOAD"))
            ((string-equal word "disp") (setq token "KW_DISP"))
            ((string-equal word "true") (setq token "KW_TRUE"))
            ((string-equal word "false") (setq token "KW_FALSE"))
        )
        token
    )
)

;;checks word is a operator or not. If it is, it return the associated token otherwise empty string
(defun check-operator-token (word quad-flag)
    (let ((token "") )
        (cond
            ((string-equal word "+") (setq token "OP_PLUS"))
            ((string-equal word "-") (setq token "OP_MINUS"))
            ((string-equal word "/") (setq token "OP_DIV"))
            ((string-equal word "*") (setq token "OP_MULT"))
            ((string-equal word "(") (setq token "OP_OP"))
            ((string-equal word ")") (setq token "OP_CP"))
            ((string-equal word "**") (setq token "OP_DBLMULT"))
            ((string-equal word ",") (setq token "OP_COMMA"))
        )
        (cond 
            ((and (string-equal word "\"") (eq quad-flag 1)) (setq token "OP_OC"))
            ((and (string-equal word "\"") (eq quad-flag 0)) (setq token "OP_CC"))
        )
        token
    )
)

;;if word is a comment end mark returns T, otherwise nil
(defun is-comment (word)
    (let ((counter 0))
        (loop for c across word do (if (eq c #\Space) (setq counter (+ counter 1)) ) )
        (if (> counter 5) T nil )
    )
)

;;checks word is a value or not, if it is, it returns T otherwise nil
(defun check-value (word)
    (let ((flag T)(counter 0))
        (setq first-number-list '(1 2 3 4 5 6 7 8 9) )
        (setq number-list '(0 1 2 3 4 5 6 7 8 9))
        (loop for c across word 
            do(if (and (eq counter 0) (equal (find (digit-char-p c) first-number-list) nil) ) (setq flag nil) )
            do(if (and (> counter 0) (equal (find (digit-char-p c) number-list) nil) ) (setq flag nil) )
            do(setq counter (+ counter 1))
        )
        (if (string-equal word "0") (setq flag T))
        flag
    )
)

;;checks word is a identifier or not, if it is, it returns T otherwise nil
(defun check-identifier (word)
    (let ((flag T) (counter 0))
        (setq number-list '(0 1 2 3 4 5 6 7 8 9))
        (setq )
        (setq word (string-downcase word) )
        (loop for c across word 
            do(if (and (eq counter 0) (eq (alpha-char-p c) nil) ) (setq flag nil) )
            do(if (and (> counter 0) (and (not (alpha-char-p c)) (equal (find (digit-char-p c) number-list) nil) ) ) (setq flag nil) )
            do(setq counter (+ counter 1))
        )
        flag
    )
)


;;analyses all words and adds associated tokens to token-list
(defun lexer-analyser (myList)
    (let ( (quad-counter 0) (loop-index 0) (quo-flag 0) (word "") (quo-end-flag 0) (word-added-flag 0) (temp-char #\0) (error-flag 1)  )
        (loop for loop-index from 0 to (- (length myList) 1) 
            do(setq word (nth loop-index myList))
            do(if (not (eq (search-comment word) -1)) (setq quo-flag 1))
            do(if (eq (is-comment word) T) (setq quo-end-flag 1))         
            do(cond  ;;gets keyword token
                ( (and (eq quo-flag 0) (not (string-equal (check-keyword-token word) "") ) ) (setq *token-list* (append *token-list* (list (check-keyword-token word)))) (setq word-added-flag 1) )
            )
            do(cond ;;gets operator token
                ( (and (and (eq quo-flag 0) (eq word-added-flag 0)) (not (string-equal (check-operator-token word (mod quad-counter 2) ) "") )) (if (string-equal word "\""  ) (setq quad-counter (+ quad-counter 1)) ) (setq *token-list* (append *token-list* (list (check-operator-token word (mod quad-counter 2) )))) (setq word-added-flag 1)  )
            )
            do(cond ;;gets value token
                ( 
                    (and (eq quo-flag 0) (eq word-added-flag 0))
                    (if (eq (check-value word) T) (setq *token-list* (append *token-list* (list "VALUE") )))
                    (if (eq (check-value word) T) (setq word-added-flag 1))
                )
            )
            do(cond ;;gets identifier token
                (
                     (and (eq quo-flag 0) (eq word-added-flag 0))
                     (if (eq (check-identifier word) T) (setq *token-list* (append *token-list* (list "IDENTIFIER") )))
                     (if (eq (check-identifier word) T) (setq word-added-flag 1))
                )
            )
            do(if (and (and (eq quo-flag 0) (eq word-added-flag 0)) ) (setq *token-list* (append *token-list* (list (concatenate 'string "SYNTAX_ERROR " word " cannot be tokenized") )) )) ;;if any token is not taken or it not on a comment line, gets an error token associated with the  token
            do(cond ((and (eq quo-flag 1) (eq quo-end-flag 1) ) (setq *token-list* (append *token-list* (list "COMMENT"))) (setq quo-flag 0) (setq quo-end-flag 0) (setq error-flag 0)  ) )  
            do(setq word-added-flag 0)
        )
    )
)

(defun run-with-file (filename)
    (setq str (getFile filename))
    (setq *token-check-list* (fill-token-check-list str))
    (setq *token-check-list* (remove-extra-whitespaces-from-token-list *token-check-list*))
    (lexer-analyser *token-check-list*)
    (loop for token in *token-list* do (print token))
)

(defun run-with-expression (expression)
    (let ((str ""))
        (if (eq (check-fullwhitespace-line expression) T) (setq str (concatenate 'string str (removeExtraWhiteSpaces (addWhiteSpaces expression)) )))
        (if (not (eq (search-comment expression) -1)) (setq str (concatenate  'string str "                            ;;&& " )))
        (if (eq (check-fullwhitespace-line expression) T) (setq str (concatenate  'string str " " )) )
        (setq *token-check-list* (fill-token-check-list str))
        (setq *token-check-list* (remove-extra-whitespaces-from-token-list *token-check-list*))
        (lexer-analyser *token-check-list*)
        (loop for token in *token-list* do (print token))
    )
    
)

;;program starts here
(let ((expression "") )
    (cond
        ( ;;if file name have not entered as an arguments it analyse the code in the file
            (eq (nth 0 *args*) nil) 
            (write-line "Type the expression that you want to analyse (if you want to analyse a file, you should have entered it as an argument like 'clisp gpp_lexer.lisp inputFile.g++')")
            (write-line "If you want to terminate program just enter empty string")
            (with-open-file (str "parsed_lisp.txt"
                    :direction :output
                    :if-exists :supersede
                    :if-does-not-exist :create)
                (format str "" ))
            (loop
                (setq expression (read-line))              
                (if (eq (length expression) 0) (return 0) )             
                (run-with-expression expression)
                (setq expression "")
                (write-line "")
                (with-open-file (str "parsed_lisp.txt"
                    :direction :output
                    :if-exists :append
                    :if-does-not-exist :create)
                (loop for c in *token-list* do(format str "~a~%" c) ))
                (setq *token-check-list* nil)
                (setq *token-list* nil)
            )
        )
        ( ;;if file name entered as an arguments it analyse the code in the file
            T
            (run-with-file (string-trim '(#\Space #\Tab #\Newline) (nth 0 *args*)))
            (with-open-file (str "parsed_lisp.txt"
                    :direction :output
                    :if-exists :supersede
                    :if-does-not-exist :create)
            (loop for c in *token-list* do(format str "~a~%" c) )) 
        )
    )
)
