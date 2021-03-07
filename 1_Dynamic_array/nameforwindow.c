#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_YEAR_DURATION	10	// 기간
#define LINEAR_SEARCH 0
#define BINARY_SEARCH 1

// 구조체 선언
typedef struct {
	char	name[20];		// 이름
	char	sex;			// 성별 M or F
	int		freq[MAX_YEAR_DURATION]; // 연도별 빈도
} tName;

typedef struct {
	int		len;		// 배열에 저장된 이름의 수
	int		capacity;	// 배열의 용량 (배열에 저장 가능한 이름의 수)
	tName	*data;		// 이름 배열의 포인터
} tNames;

////////////////////////////////////////////////////////////////////////////////
// 함수 원형 선언(declaration)

// 연도별 입력 파일을 읽어 이름 정보(이름, 성별, 빈도)를 이름 구조체에 저장
// 이미 구조체에 존재하는(저장된) 이름은 해당 연도의 빈도만 저장
// 새로 등장한 이름은 구조체에 추가
// 주의사항: 동일 이름이 남/여 각각 사용될 수 있으므로, 이름과 성별을 구별해야 함
// names->capacity는 2배씩 증가
// 선형탐색(linear search) 버전
/*먼저, fp에서 한 줄 읽고, 이름과 성별 빈도로 나눔(scanf사용)
그 후, 선형탐색으로 이름 및 성별 존재하는 지 체크
존재할 시 해당 frequency값만 수정, 존재하지 않으면 names->data에 새로 추가
*/
void load_names_lsearch(FILE *fp, int year_index, tNames *names)
{
	while (!feof(fp)) { //file의 끝까지 loop
		char buff[100];
		fgets(buff, 100, fp);
		char temp_name[20];
		char temp_sex;
		int temp_freq;
		char *ptr = strchr(buff, ',');
		while (ptr) {
			*ptr = ' ';
			printf("%s\n", ptr + 1);
			ptr = strchr(ptr + 1, ',');
		}
		sscanf(temp, "%s %c %d", temp_name, &temp_sex, &frequency);

		if (!year_index) { // 2009년인 경우 탐색 필요 없이 data 추가만 해주면 된다.
			memcpy(names->data[names->len].name, temp_name, strlen(temp_name));
			names->data[names->len].name[strlen(temp_name)] = '\n';
			names->data[names->len].sex = temp_sex;
			memset(names->data[names->len].freq, 0, sizeof(names->data[names->len].freq)); // 모든 연도의 frequency값 0으로 초기화
			names->data[names->len].freq[year_index] = temp_freq;
			(names->len)++;
		}
		else { // 2010년 이상인 경우
			char existed = false;
			for (int i = 0; i < names->len; i++) {
				if (strcmp(names->data[i].name, temp_name) && names->data[i].sex == temp_sex) { // 같은 data가 이미 존재한다면
					names->data[i].freq[year_index] = temp_freq;
					existed = true;
				}
			}
			if (!existed) { //기존에 없던 사람인 경우
				memcpy(names->data[names->len].name, temp_name, strlen(temp_name));
				names->data[names->len].name[strlen(temp_name)] = '\n';
				names->data[names->len].sex = temp_sex;
				memset(names->data[names->len].freq, 0, sizeof(names->data[names->len].freq)); // 모든 연도의 frequency값 0으로 초기화
				names->data[names->len].freq[year_index] = temp_freq;
				(names->len)++;
			}
		}
		if (names->capacity == names->len) { //용량이 다 찼다면 2배로 늘린다.
			realloc(names->data, (names->capacity) * 2 * sizeof(tName));
			(names->capacity) *= 2;
		}
	}
}

// 이진탐색(binary search) 버전
void load_names_bsearch(FILE *fp, int year_index, tNames *names)
{

}

// 구조체 배열을 화면에 출력
void print_names(tNames *names, int num_year) {
	for (int i = 0; i < names->len; i++) {
		printf("%s\t", names->data[i].name);
		for (int j = 0; j < MAX_YEAR_DURATION; j++) {
			printf("%d\t", names->data[i].freq[j]);
		}
		printf('\n');
	}
}

// qsort를 위한 비교 함수
// 정렬 기준 : 이름(1순위), 성별(2순위)
int compare(const void *n1, const void *n2) {
	
}

////////////////////////////////////////////////////////////////////////////////
// 함수 정의 (definition)

// 이름 구조체를 초기화
// len를 0으로, capacity를 1로 초기화
// return : 구조체 포인터
tNames *create_names(void)
{
	tNames *pnames = (tNames *)malloc( sizeof(tNames));
	
	pnames->len = 0;
	pnames->capacity = 1;
	pnames->data = (tName *)malloc(pnames->capacity * sizeof(tName));

	return pnames;
}

// 이름 구조체에 할당된 메모리를 해제
void destroy_names(tNames *pnames)
{
	free(pnames->data);
	pnames->len = 0;
	pnames->capacity = 0;

	free(pnames);
}

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	tNames *names;
	int mode;
	
	FILE *fp;
	int num_year = 0;
	
	
	mode = LINEAR_SEARCH;
	//mode = BINARY_SEARCH;
	
	// 이름 구조체 초기화
	names = create_names();

	// 첫 연도 알아내기 "yob2009.txt" -> 2009
	int start_year = 2009;
	
	for (int i = 0; i < 2; i++)
	{
		num_year++;
		if(i==0)fp = fopen("yob2009.txt", "r");
		else fp = fopen("yob2010.txt", "r");
		if( !fp) {
			fprintf( stderr, "cannot open file : %s\n", "fuck");
			return 1;
		}

		int year = start_year+i; // ex) "yob2009.txt" -> 2009
		
		if (mode == LINEAR_SEARCH)
		{
			// 연도별 입력 파일(이름 정보)을 구조체에 저장
			// 선형탐색 모드
			load_names_lsearch( fp, year-start_year, names);
		
		}
		else // (mode == BINARY_SEARCH)
		{
			// 이진탐색 모드
			load_names_bsearch( fp, year-start_year, names);
			
			// 정렬 (이름순 (이름이 같은 경우 성별순))
			qsort( names->data, names->len, sizeof(tName), compare);
		}
		fclose( fp);

	}
	
	// 정렬 (이름순 (이름이 같은 경우 성별순))
	qsort( names->data, names->len, sizeof(tName), compare);
	
	// 이름 구조체를 화면에 출력
	print_names( names, num_year);

	// 이름 구조체 해제
	destroy_names( names);
	
	return 0;
}

