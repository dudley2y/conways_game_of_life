build:
	g++ main.cpp
	./a.out

debug: 

	g++ main.cpp
	./a.out -d

git-readme:
	git add README.md
	git commit -m "fixing readme"
	git push
