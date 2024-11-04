ready/report.pdf: report/report.pdf
	mkdir -p ./ready
	cp report/report.pdf ready/report.pdf

ready/stud-unit-test-report-prev.json: src/stud-unit-test-report-prev.json
	mkdir -p ./ready
	cp src/stud-unit-test-report-prev.json ready/stud-unit-test-report-prev.json

ready/main-cli-debug.py: src/test.py
	mkdir -p ./ready
	cp src/* ready/

ready/app-cli-debug:

.PHONY: clean
clean:
	echo OK
