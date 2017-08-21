<?php
$br = (php_sapi_name() == "cli")? "":"<br>";

if(!extension_loaded('excelwriter')) {
	dl('excelwriter.' . PHP_SHLIB_SUFFIX);
}
echo '创建ExcelBook对象', PHP_EOL;
$book = new ExcelBook('test.xlsx');
echo '增加Sheet', PHP_EOL;
$sheet = $book->addSheet('test');
echo '增加列', PHP_EOL;
$sheet->addColumn('ID', 'id', ExcelColumn::TYPE_NUMBER);
$sheet->addColumn('姓名', 'name');
$sheet->addColumn('手机', 'mobile');
$sheet->addColumn('金额', function($data){
    return $data['money'] - $data['frozen_money'];
}, ExcelColumn::TYPE_NUMBER);
$data = [
    ['id' => 1, 'name' => 'ChienHo', 'mobile' => '134*******0', 'money' => 100, 'frozen_money' => 1],
    ['id' => 2, 'name' => 'Johnny', 'mobile' => '138*******3', 'money' => 13, 'frozen_money' => -1],
    ['id' => 3, 'name' => 'Ban', 'mobile' => '134*******2', 'money' => 200, 'frozen_money' => 0],
];
$sheet->writeByArray($data);
if($error = $book->save()) {
    echo '已写入', PHP_EOL;
} else {
    echo '写入失败:', $error, PHP_EOL;
}
