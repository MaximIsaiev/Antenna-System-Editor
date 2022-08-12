#include "antsystemeditorbackend.h"
#include <QDir>
#include <QDebug>
#include <QStandardItem>
#include <mathutils.h>

namespace dfpost {

static const int DEFAULT_ID = -1;
static constexpr int MGHZ_TO_GHZ = 1000000;
static const int DEFAULT_COUNT_DELETE = 1;
static const QString NOT_FOUND_INDEX_ERROR = QObject::tr("Индекс не найден");
static const QString CANNOT_BE_CREATED_SUBPANEL_ERROR = QObject::tr("Нельзя создать подпанель для подпанели");
static const QString CANNOT_BE_CREATED_ERROR = QObject::tr("Невозможно создать панель");
static const QString SAME_NAME_ERROR = QObject::tr("Дублирующееся имя");
static const QString DELETE_ELEMENT_ERROR = QObject::tr("ошибка при удалении элемента");
static const QString SUCCESSFULL_CREATE_INFO = QObject::tr("Успешное создание панели");


AntSystemEditorBackend::AntSystemEditorBackend(QObject *parent) :
    QObjectInheritableBaseObject<CustomUiBackend>(parent)
{
    m_antSystemElementsModel = new AntSystemElementsModel(this);
    m_antSystemTreeViewModel = new AntSystemTreeViewModel(this);

    // устанавливаем связь между изменениями в таблице и изменениями в топологии
    connect(m_antSystemElementsModel, &AntSystemElementsModel::dataChanged, this, &AntSystemEditorBackend::updateAntSystemElements);
}


AntSystemElementsModel *AntSystemEditorBackend::elementsModel() const
{
    return m_antSystemElementsModel;
}

QString AntSystemEditorBackend::currAntennaName() const
{
    return m_currAntennaName;
}

void AntSystemEditorBackend::setCurrAntennaName(const QString &newCurrAntennaName)
{
    m_currAntennaName = newCurrAntennaName;
    if (!m_currAntennaName.isEmpty()) {
        drawPointXYZ();
        drawPointXYZBackup();
        setElementsDataForModel();
        setCurrentAntSystem();
    }

}

float AntSystemEditorBackend::pointX() const
{
    return m_PointX;
}

void AntSystemEditorBackend::setPointX(float newPointX)
{
    m_PointX = newPointX;
}

float AntSystemEditorBackend::pointY() const
{
    return m_PointY;
}

void AntSystemEditorBackend::setPointY(float newPointY)
{
    m_PointY = newPointY;
}

float AntSystemEditorBackend::pointZ() const
{
    return m_PointZ;
}

void AntSystemEditorBackend::setPointZ(float newPointZ)
{
    m_PointZ = newPointZ;
}

Frequency AntSystemEditorBackend::begFreq() const
{
    size_t systemIndex { 0 };

    if (findIndexByName(systemIndex, m_topology))
        return m_topology.at(systemIndex).begFreq();

    return Frequency::Hz(0);
}

void AntSystemEditorBackend::setBegFreq(const Frequency &newBegFreq)
{
    size_t systemIndex { 0 };

    // устанавлиаем нижнюю границу частоты
    if (findIndexByName(systemIndex, m_topology)) {
        m_topology.at(systemIndex).setBegFreq(newBegFreq);
        emit begFreqChanged();
    } else {
        writeError(NOT_FOUND_INDEX_ERROR);
    }
}

Frequency AntSystemEditorBackend::endFreq() const
{
    size_t systemIndex { 0 };

    if (findIndexByName(systemIndex, m_topology))
        return m_topology.at(systemIndex).endFreq();

    return Frequency::Hz(0);
}

void AntSystemEditorBackend::setEndFreq(const Frequency &newEndFreq)
{
    size_t systemIndex { 0 };

    // устанавлиаем верхнюю границу частоты
    if (findIndexByName(systemIndex, m_topology)) {
        m_topology.at(systemIndex).setEndFreq(newEndFreq);
        emit endFreqChanged();
    } else {
        writeError(NOT_FOUND_INDEX_ERROR);
    }

}

void AntSystemEditorBackend::writeError(const QString &newError)
{
    loggerQt()->error(newError);
    m_exception = newError;
    emit errorLogged();
}

QString AntSystemEditorBackend::error() const
{
    return m_exception;
}

void AntSystemEditorBackend::writeInformation(const QString &newInformation)
{
    loggerQt()->info(newInformation);
    m_information = newInformation;
    emit informationLogged();
}

QString AntSystemEditorBackend::information() const
{
    return m_information;
}

void AntSystemEditorBackend::updateAntSystemElements()
{
    size_t systemIndex { 0 };

    if (findIndexByName(systemIndex, m_topology))
        m_topology.at(systemIndex).setElements(m_antSystemElementsModel->elements());
    else
        writeError(NOT_FOUND_INDEX_ERROR);

    if (findIndexByName(systemIndex, m_topologyBackup))
        m_topologyBackup.at(systemIndex).setElements(m_antSystemElementsModel->elements());
    else
        writeError(NOT_FOUND_INDEX_ERROR);

    sortAntSystemId();

    emit chartsNeedsClear();
    drawPointXYZ();
    drawPointXYZBackup();
}

void AntSystemEditorBackend::sortAntSystemId()
{
    std::vector<ElementOfAntennaSystem> elements;
    uint tempId = 1;
    uint mainId = 1;
    for (auto && currSystem : m_topology) {
        currSystem.setId(mainId);
        mainId++;
        elements = currSystem.element();
        for (auto && currElement: elements) {
            currElement.id = tempId;
            tempId++;
        }
        currSystem.setElements(elements);
    }
    tempId = 1;
    for (auto && currSystem : m_topologyBackup) {
        elements = currSystem.element();
        for (auto && currElement: elements) {
            currElement.id = tempId;
            tempId++;
        }
        currSystem.setElements(elements);
    }
}

bool AntSystemEditorBackend::findIndexByName(size_t &index, const std::vector<AntSystemTopology> &topology) const
{
    const std::string currName = currAntennaName().toStdString();

    auto it = std::find_if(topology.begin(), topology.end(), [currName](const AntSystemTopology &value){
        return value.name() == currName;
    });

    if (it != topology.end()) {
        index = std::distance(topology.begin(), it);
        return true;
    }
    return false;
}

bool AntSystemEditorBackend::removeChildById(size_t &index, uint32_t child, const std::vector<AntSystemTopology> &topology)
{
    auto it = std::find_if(topology.begin(), topology.end(), [child](const AntSystemTopology &value) {
        return value.id() == child;
    });
    if (it != topology.end()) {
        index = std::distance(topology.begin(), it);
        return true;
    }
    return false;
}


AntSystemTreeViewModel *AntSystemEditorBackend::antSystemName() const
{
    return m_antSystemTreeViewModel;
}



float AntSystemEditorBackend::roll() const
{
    size_t systemIndex { 0 };

    if (findIndexByName(systemIndex, m_topology))
        return m_topology.at(systemIndex).roll();

    return 0.0f;
}

void AntSystemEditorBackend::setRoll(float newRoll)
{
    size_t systemIndex { 0 };

    // устанавливаем крен для текущей АС
    if (findIndexByName(systemIndex, m_topology)) {
        m_topology.at(systemIndex).setRoll(newRoll);
        drawPointXYZ();
        emit rollChanged();
        return;
    }

    writeError(NOT_FOUND_INDEX_ERROR);
}

float AntSystemEditorBackend::pitch() const
{
    size_t systemIndex { 0 };

    if (findIndexByName(systemIndex, m_topology))
        return m_topology.at(systemIndex).pitch();

    return 0.0f;
}

void AntSystemEditorBackend::setPitch(float newPitch)
{
    size_t systemIndex { 0 };

    // устанавливаем тангаж для текущей АС
    if (findIndexByName(systemIndex, m_topology)) {
        m_topology.at(systemIndex).setPitch(newPitch);
        drawPointXYZ();
        emit pitchChanged();
        return;
    }

    writeError(NOT_FOUND_INDEX_ERROR);
}

float AntSystemEditorBackend::yaw() const
{
    size_t systemIndex { 0 };

    if (findIndexByName(systemIndex, m_topology))
        return m_topology.at(systemIndex).yaw();

    return 0.0f;
}

void AntSystemEditorBackend::setYaw(float newYaw)
{
    size_t systemIndex { 0 };

    // устанавливаем рыскание для текущей АС
    if (findIndexByName(systemIndex, m_topology)) {
        m_topology.at(systemIndex).setYaw(newYaw);
        drawPointXYZ();
        emit yawChanged();
        return;
    }

    writeError(NOT_FOUND_INDEX_ERROR);
}


void AntSystemEditorBackend::setCurrentAntSystem()
{
    size_t systemIndex { 0 };

    // устанавливаем значения крена, тангажа, рыскания и диапазона частот для выбранной системы
    if (findIndexByName(systemIndex, m_topology)) {
        auto &currentSystem = m_topology.at(systemIndex);

        setRoll(currentSystem.roll());
        emit rollChanged();
        setPitch(currentSystem.pitch());
        emit pitchChanged();
        setYaw(currentSystem.yaw());
        emit yawChanged();
        setBegFreq(currentSystem.begFreq());
        emit begFreqChanged();
        setEndFreq(currentSystem.endFreq());
        emit endFreqChanged();
        setMinAz(currentSystem.getMinAz());
        emit minAzChanged();
        setMaxAz(currentSystem.getMaxAz());
        emit maxAzChanged();
        setMinAng(currentSystem.getMinAng());
        emit minAngChanged();
        setMaxAng(currentSystem.getMaxAng());
        emit maxAngChanged();
        return;
    }

    writeError(NOT_FOUND_INDEX_ERROR);
}

float AntSystemEditorBackend::minAng() const
{
    size_t systemIndex { 0 };

    if (findIndexByName(systemIndex, m_topology))
        return m_topology.at(systemIndex).getMinAng();

    return 0.0f;
}

void AntSystemEditorBackend::setMinAng(float newMinAng)
{
    size_t systemIndex { 0 };

    // устанавливаем минимальное значение угла для текущей АС
    if (findIndexByName(systemIndex, m_topology)) {
        m_topology.at(systemIndex).setMinAng(newMinAng);
        emit minAngChanged();
        return;
    }

    writeError(NOT_FOUND_INDEX_ERROR);
}

float AntSystemEditorBackend::maxAng() const
{
    size_t systemIndex { 0 };

    if (findIndexByName(systemIndex, m_topology))
        return m_topology.at(systemIndex).getMaxAng();

    return 0.0f;
}

void AntSystemEditorBackend::setMaxAng(float newMaxAng)
{
    size_t systemIndex { 0 };

    // устанавливаем максимальное значение угла для текущей АС
    if (findIndexByName(systemIndex, m_topology)) {
        m_topology.at(systemIndex).setMaxAng(newMaxAng);
        emit maxAngChanged();
        return;
    }

    writeError(NOT_FOUND_INDEX_ERROR);
}

float AntSystemEditorBackend::maxAz() const
{
    size_t systemIndex { 0 };

    if (findIndexByName(systemIndex, m_topology))
        return m_topology.at(systemIndex).getMaxAz();

    return 0.0f;
}

void AntSystemEditorBackend::setMaxAz(float newMaxAz)
{
    size_t systemIndex { 0 };

    // устанавливаем максимальное значение азимута для текущей АС
    if (findIndexByName(systemIndex, m_topology)) {
        m_topology.at(systemIndex).setMaxAz(newMaxAz);
        emit maxAzChanged();
        return;
    }

    writeError(NOT_FOUND_INDEX_ERROR);
}

float AntSystemEditorBackend::getMinAz() const
{
    size_t systemIndex { 0 };

    if (findIndexByName(systemIndex, m_topology))
        return m_topology.at(systemIndex).getMinAz();

    return 0.0f;
}

void AntSystemEditorBackend::setMinAz(float newMinAz)
{
    size_t systemIndex { 0 };

    // устанавливаем минимальное значение азимута для текущей АС
    if (findIndexByName(systemIndex, m_topology)) {
        m_topology.at(systemIndex).setMinAz(newMinAz);
        emit minAzChanged();
        return;
    }

    writeError(NOT_FOUND_INDEX_ERROR);
}

void AntSystemEditorBackend::deleteAntSystem(const QModelIndex &index)
{
    if (m_antSystemTreeViewModel->removeRows(index.row(), 1, index.parent())) {
        const std::string currName = currAntennaName().toStdString();
        std::vector<int> childs;
        // удаляем конкретный элемент в основной топологии
        for (auto it = m_topology.cbegin(); it != m_topology.end(); it++) {
            if (it->name() == currName) {

                // если пользователь хочет удалить целую группу элементов
                if (it->isLiter()) {
                    for(auto &&child: it->childs()) {
                        size_t index = { 0 };
                        removeChildById(index, child, m_topology);
                        m_topology.erase(m_topology.cbegin() + index);
                    }
                }
                m_topology.erase(it);
                break;
            }
        }

        // удаляем конкретный элемент в запасной топологии
        for (auto it = m_topologyBackup.cbegin(); it != m_topologyBackup.end(); it++) {
            if (it->name() == currName) {

                // если пользователь хочет удалить целую группу элементов
                if (it->isLiter()) {
                    for(auto &&child: it->childs()) {
                        size_t index = { 0 };
                        removeChildById(index, child, m_topologyBackup);
                        m_topologyBackup.erase(m_topologyBackup.cbegin() + index);
                    }
                }
                m_topologyBackup.erase(it);
                break;
            }
        }
        // удаляем все элементы с модели
        m_antSystemElementsModel->clear();
        emit chartsNeedsClear();
        return;
    }

    writeError(DELETE_ELEMENT_ERROR);
}

bool AntSystemEditorBackend::createNewAntSystem(const QString &newAntennaSystemName)
{
    AntSystemTopology system;

    // проверка на одинаковое имя
    for (auto && currSystem : m_topology) {
        if (currSystem.name() == newAntennaSystemName.toStdString()) {
            writeError(SAME_NAME_ERROR);
            return false;
        }
    }

    // добавляем систему в топологию
    system.setId(DEFAULT_ID);
    system.setName(newAntennaSystemName.toStdString());
    m_topology.push_back(system);
    m_topologyBackup.push_back(system);

    // добавляем систему в модель
    if (m_topology.size() == 1U) {
        m_antSystemTreeViewModel->setTopology(m_topology);
    } else {
        QStandardItem *group = new QStandardItem;
        group->setText(newAntennaSystemName);
        m_antSystemTreeViewModel->appendRow(group);
    }

    if (m_antSystemTreeViewModel->rowCount() == static_cast<int>(m_topology.size())) {
        writeInformation(SUCCESSFULL_CREATE_INFO);
        // делаем сквозную нумерацию
        sortAntSystemId();
        return true;
    }

    writeError(CANNOT_BE_CREATED_ERROR);
    return false;
}

bool AntSystemEditorBackend::createNewSubPanel(const QString &newSubPanelName, const QModelIndex index)
{
    AntSystemTopology system;

    std::vector<unsigned int> child;

    // проверка на одинаковое имя
    for (auto && currSystem : m_topology) {
        if (currSystem.name() == newSubPanelName.toStdString()) {
            writeError(SAME_NAME_ERROR);
            return false;
        }
    }

    // проверка на валидность родителя
    if (index.parent().isValid()) {
        writeError(CANNOT_BE_CREATED_SUBPANEL_ERROR);
        return false;
    }

    // добавляем систему в топологию
    system.setId(DEFAULT_ID);
    system.setName(newSubPanelName.toStdString());
    m_topology.push_back(system);
    m_topologyBackup.push_back(system);

    // добавляем систему в модель
    QStandardItem *group = new QStandardItem;
    group->setText(newSubPanelName);
    m_antSystemTreeViewModel->itemFromIndex(index)->appendRow(group);

    if (m_antSystemTreeViewModel->itemFromIndex(index)->rowCount() > 0) {

        // делаем сквозную нумерацию
        sortAntSystemId();

        size_t systemIndex;
        if (!findIndexByName(systemIndex, m_topology)) {
            writeError(NOT_FOUND_INDEX_ERROR);
            return false;
        }
        auto &currentSystem = m_topology.at(systemIndex);
        child = currentSystem.childs();
        child.push_back(m_topology.back().id());
        currentSystem.setIsLiter(true);
        currentSystem.setChilds(child);
        currentSystem.setIsLiter(true);
        currentSystem.setChilds(child);
        writeInformation(SUCCESSFULL_CREATE_INFO);
        return true;
    }

    writeError(CANNOT_BE_CREATED_SUBPANEL_ERROR);
    return false;
}

void AntSystemEditorBackend::createNewAntSystemElement()
{
    std::vector<ElementOfAntennaSystem> elements;
    ElementOfAntennaSystem element;
    size_t systemIndex { 0 } ;

    if (findIndexByName(systemIndex, m_topology)) {
        auto &currentSystem = m_topology.at(systemIndex);

        float centerX = NULL_FLOAT;
        float centerY = NULL_FLOAT;
        float centerZ = NULL_FLOAT;

        if (!currentSystem.element().empty()) {
            for (auto && currElement: currentSystem.element()) {
                centerX += currElement.x;
                centerY += currElement.y;
                centerZ += currElement.z;
            }
            // расчет центра масс
            centerX = centerX / currentSystem.element().size();
            centerY = centerY / currentSystem.element().size();
            centerZ = centerZ / currentSystem.element().size();
        }
        // задание центра масс
        element.x = centerX;
        element.y = centerY;
        element.z = centerZ;

        elements = currentSystem.element();
        elements.push_back(element);
        currentSystem.setElements(elements);

        if (findIndexByName(systemIndex, m_topologyBackup)) {
            currentSystem = m_topologyBackup.at(systemIndex);
            elements = currentSystem.element();
            elements.push_back(element);
            currentSystem.setElements(elements);

            m_antSystemElementsModel->setElements(elements);
            updateAntSystemElements();
            return;
        }
    }

    writeError(NOT_FOUND_INDEX_ERROR);
}

void AntSystemEditorBackend::deleteAntSystemElement(int index)
{
    const std::string currName = currAntennaName().toStdString();
    std::vector<ElementOfAntennaSystem> elements;

    for (auto && currSystem : m_topology) {
        if (currSystem.name() == currName) {
            elements = currSystem.element();
            int localIndex = 0;

            for (auto it = elements.begin(); it != elements.end(); it++) {
                if (localIndex == index) {
                    elements.erase(it);
                    break;
                }
                localIndex++;
            }
            break;
        }
    }

    m_antSystemElementsModel->setElements(elements);
    updateAntSystemElements();
}

void AntSystemEditorBackend::deleteAllAntSystemElements()
{
    const std::vector<ElementOfAntennaSystem> empty;
    size_t systemIndex { 0 };

    if (findIndexByName(systemIndex, m_topology)) {
        m_topology.at(systemIndex).setElements(empty);

        if (findIndexByName(systemIndex, m_topologyBackup)) {
            // делаем сквозную нумерацию
            sortAntSystemId();
            // очищаем графики
            emit chartsNeedsClear();

            // очищаем данные таблицы координат
            m_topologyBackup.at(systemIndex).setElements(empty);
            m_antSystemElementsModel->setElements(empty);
            return;
        }
    }
    writeError(NOT_FOUND_INDEX_ERROR);
}

void AntSystemEditorBackend::setElementsDataForModel()
{
    std::vector<ElementOfAntennaSystem> elements;

    // ищем выбранную АС и ее элементы передаем в класс модели
    size_t systemIndex { 0 };

    if (findIndexByName(systemIndex, m_topology)) {
        elements = m_topology.at(systemIndex).element();
        m_antSystemElementsModel->setElements(elements);
        return;
    }

    writeError(NOT_FOUND_INDEX_ERROR);
}

bool AntSystemEditorBackend::updateAntSystemName(const QModelIndex &index, const QVariant &newAntSystemName)
{
    // проверка на одинаковое имя
    for (auto && currSystem : m_topology) {
        if (currSystem.name() == newAntSystemName.toString().toStdString()) {
            writeError(SAME_NAME_ERROR);
            return false;
        }
    }

    size_t systemIndex { 0 };

    if (!findIndexByName(systemIndex, m_topology)) {
        writeError(NOT_FOUND_INDEX_ERROR);
        return false;
    }

    m_topology.at(systemIndex).setName(newAntSystemName.toString().toStdString());

    if (!findIndexByName(systemIndex, m_topologyBackup)) {
        writeError(NOT_FOUND_INDEX_ERROR);
        return false;
    }

    m_topologyBackup.at(systemIndex).setName(newAntSystemName.toString().toStdString());

    // обновляем модель с именами АС
    m_antSystemTreeViewModel->setData(index, newAntSystemName);

    // задаем как бы новую текущую АС (по сути та же самая, только с другим именем)
    setCurrAntennaName(newAntSystemName.toString());
    return true;
}

void AntSystemEditorBackend::drawGroupElements(const Location location, const Direction direction, double count, double diameter)
{
    std::vector<ElementOfAntennaSystem> elements;
    ElementOfAntennaSystem element;
    double currentDirection = 0.0;

    switch (direction) {
    case Clockwise:
        currentDirection = -1.0;
        break;
    case CounterClockwise:
        currentDirection = 1.0;
        break;
    }

    switch (location)  {
    case RoundLocation:
        for (auto i = 0; i < count; i++) {
            element.x = diameter / 2 * cos(degToRad(currentDirection * i * 360 / count)) * currentDirection;
            element.y = diameter / 2 * sin(degToRad(currentDirection * i * 360 / count)) * currentDirection;
            element.z = 0.0f;
            elements.push_back(element);
        }
        break;
    case FirstInCenter:
        element.x = 0.0f;
        element.y = 0.0f;
        element.z = 0.0f;
        elements.push_back(element);
        for (auto i = 0; i < count - 1; i++) {
            element.x = diameter / 2 * cos(degToRad(currentDirection * i * 360 / (count - 1))) * currentDirection;
            element.y = diameter / 2 * sin(degToRad(currentDirection * i * 360 / (count - 1))) * currentDirection;
            element.z = 0.0f;
            elements.push_back(element);
        }
        break;
    case LastInCenter:
        for (auto i = 0; i < count - 1; i++) {
            element.x = diameter / 2 * cos(degToRad(currentDirection * i * 360 / (count - 1))) * currentDirection;
            element.y = diameter / 2 * sin(degToRad(currentDirection * i * 360 / (count - 1))) * currentDirection;
            element.z = 0.0f;
            elements.push_back(element);
        }
        element.x = 0.0f;
        element.y = 0.0f;
        element.z = 0.0f;
        elements.push_back(element);
        break;
    }
    m_antSystemElementsModel->setElements(elements);
    updateAntSystemElements();
}

int AntSystemEditorBackend::countAntSystems()
{
    return static_cast<int>(m_topology.size());
}

void AntSystemEditorBackend::createNewFile()
{
    m_topology.clear();
    m_topologyBackup.clear();
    m_antSystemTreeViewModel->clear();
    m_antSystemElementsModel->clear();
    emit chartsNeedsClear();
}


void AntSystemEditorBackend::saveAntSystemFile(const QUrl filePath)
{
    const auto strPath = QDir::toNativeSeparators(filePath.toLocalFile());
    m_antSystemFilePath = strPath;
    m_FileHandling.setTopology(m_topology);
    m_FileHandling.save(strPath.toStdString());
}

void AntSystemEditorBackend::loadAntSystemFile(const QUrl filePath)
{
    createNewFile();
    const auto strPath = QDir::toNativeSeparators(filePath.toLocalFile());
    m_antSystemFilePath = strPath;

    // считываем данные из файла
    m_FileHandling.load(strPath.toStdString());


    // получили вектор класса АС из файла
    m_topology = m_FileHandling.topology();
    m_topologyBackup = m_FileHandling.topologyBackup();
    m_antSystemTreeViewModel->setTopology(m_topology);


    // обнуляем остатки антенных характеристик
    setCurrAntennaName(QString());
    setRoll(NULL_FLOAT);
    setPitch(NULL_FLOAT);
    setYaw(NULL_FLOAT);
    setBegFreq(Frequency::Hz(0));
    setEndFreq(Frequency::Hz(0));
    setMinAz(NULL_FLOAT);
    setMaxAz(NULL_FLOAT);
    setMinAng(NULL_FLOAT);
    setMaxAng(NULL_FLOAT);

    m_antSystemId = 100;
    emit chartsNeedsClear();
}


// отрисовка всех видов (кроме главного)
void AntSystemEditorBackend::drawPointXYZ()
{
    size_t systemIndex { 0 };

    if (findIndexByName(systemIndex, m_topology)) {
        auto &currentSystem = m_topology.at(systemIndex);

        // если элементов нет, значит нужно очистить график
        if (currentSystem.element().empty()) {
            emit chartsNeedsClear();
            return;
        }

        for (auto && element : currentSystem.element()) {
            const float localRoll = degToRad(currentSystem.roll());
            const float localPitch = degToRad(currentSystem.pitch());
            const float localYaw = degToRad(currentSystem.yaw());
            float localX = NULL_FLOAT;
            float localY = NULL_FLOAT;
            float localZ = NULL_FLOAT;
            double sinValue;
            double cosValue;

            // крен
            sincos(localRoll, sinValue, cosValue);
            setPointX(element.x * cosValue - element.z * sinValue);
            setPointY(element.y);
            setPointZ(element.z * cosValue + element.x * sinValue);

            localX = pointX();
            localY = pointY();
            localZ = pointZ();

            // тангаж
            sincos(localPitch, sinValue, cosValue);
            setPointX(localX);
            setPointY(localY * cosValue - localZ * sinValue);
            setPointZ(localZ * cosValue + localY * sinValue);

            localX = pointX();
            localY = pointY();
            localZ = pointZ();

            // рыскание
            sincos(localYaw, sinValue, cosValue);
            setPointX(localX * cosValue - localY * sinValue);
            setPointY(localY * cosValue + localX * sinValue);
            setPointZ(localZ);
            emit pointXYZChanged();
        }
        return;
    }

    writeError(NOT_FOUND_INDEX_ERROR);
}


// отрисовка главного вида
void AntSystemEditorBackend::drawPointXYZBackup()
{
    size_t systemIndex { 0 };

    if (findIndexByName(systemIndex, m_topologyBackup)) {
        auto &currentSystem = m_topologyBackup.at(systemIndex);

        // если элементов нет, значит очищаем график
        if (currentSystem.element().empty()) {
            emit chartsNeedsClear();
            return;
        }
        for (auto && element : currentSystem.element()) {
            const float localRoll = degToRad(currentSystem.roll());
            const float localPitch = degToRad(currentSystem.pitch());
            const float localYaw = degToRad(currentSystem.yaw());
            float localX = NULL_FLOAT;
            float localY = NULL_FLOAT;
            float localZ = NULL_FLOAT;
            double sinValue = NULL_FLOAT;
            double cosValue = NULL_FLOAT;

            // крен
            sincos(localRoll, sinValue, cosValue);
            setPointX(element.x * cosValue - element.z * sinValue);
            setPointY(element.y);
            setPointZ(element.z * cosValue + element.x * sinValue);

            localX = pointX();
            localY = pointY();
            localZ = pointZ();

            // тангаж
            sincos(localPitch, sinValue, cosValue);
            setPointX(localX);
            setPointY(localY * cosValue - localZ * sinValue);
            setPointZ(localZ * cosValue + localY * sinValue);

            localX = pointX();
            localY = pointY();
            localZ = pointZ();

            // рыскание
            sincos(localYaw, sinValue, cosValue);
            setPointX(localX * cosValue - localY * sinValue);
            setPointY(localY * cosValue + localX * sinValue);
            setPointZ(localZ);

            emit pointXYZBackupChanged();
        }
        return;
    }
    writeError(NOT_FOUND_INDEX_ERROR);
}

}

